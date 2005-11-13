#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <ruby.h>
#include "dl.h"
#include "ruby_stream.hh"
#include "config.hh"
#include "dyn.hh"
#include "mlc.hh"

namespace dyn {

  <% NB_MAX_ARGUMENTS = 10 %>

  struct fun;

  struct function_loader_t
  {
    typedef void (*fun_t)();

    function_loader_t()
    {
      ruby_init();
      ruby_script("function_loader.hh");
      ruby_init_loadpath();
      ruby << "require \""
           << SOURCE_DIR
           << "/function_loader.rb"
           << "\"" << ruby::eval;
    }

    void
    include(const std::string& file)
    {
      ruby << "FunctionLoader.include \"" << file << "\"" << ruby::eval;
    }

    void
    include_dir(const std::string& dir)
    {
      ruby << "FunctionLoader.include_dir \"" << dir << "\"" << ruby::eval;
    }

    void*
    load(const std::string& kind,
         const std::string& name,
         const std::string& arguments_types,
         const std::string& header_path)
    {
      ruby << "FunctionLoader.call " << kind << ", \""
           << name << "\", [\"" <<  arguments_types << "\"], \""
           << header_path << "\"" << ruby::eval;
      return RDLPTR(ruby.last_value())->ptr;
    }

    ruby::stream ruby;
  };

  function_loader_t function_loader;

  struct generic_fun
  {
    generic_fun(const std::string& kind,
                const std::string& name,
                const std::string& header_path="") :
      kind_(kind), name_(name), header_path_(header_path) {}

    <%- NB_MAX_ARGUMENTS.times do |i| -%>

      <%- typenames = (0 .. i - 1).map { |j| "typename T#{j}" }.join(', ') -%>
      <%- arguments = (0 .. i - 1).map { |j| "const T#{j}& arg#{j}" }.join(', ') -%>
      <%- objects   = (0 .. i - 1).map { |j| "object#{j}" }.join(', ') -%>

      <%- unless typenames.empty? -%>
      template < <%= typenames %> >
      <%- end -%>
      data
      operator() (<%= arguments %>) const
      {
        typedef data (*func_t)(<%= (['const dyn::data&'] * i).join(', ') %>);
        std::string arguments_types;

      <%- i.times do |j| -%>
        const data object<%= j %> = arg<%= j %>;
        arguments_types += <%= (j.zero?)? '' : '"\", \"" + ' %>object<%= j %>.type();
      <%- end -%>

        func_t ptr = (func_t)function_loader.load(kind_, name_, arguments_types, header_path_);
        assert(ptr != 0);
        return ptr(<%= objects %>);
      }

    <%- end -%>

    const std::string kind_;
    const std::string name_;
    const std::string header_path_;
  };

  struct fun : public generic_fun
  {
    fun(const std::string& name, const std::string& header_path="") :
      generic_fun(":fun", name, header_path) {}
  };

  struct proc : public generic_fun
  {
    proc(const std::string& name, const std::string& header_path="") :
      generic_fun(":proc", name, header_path) {}
  };

  struct method_proc : public generic_fun
  {
    method_proc(const std::string& name, const std::string& header_path="") :
      generic_fun(":method_proc", name, header_path) {}
  };

  struct method_fun : public generic_fun
  {
    method_fun(const std::string& name, const std::string& header_path="") :
      generic_fun(":method_fun", name, header_path) {}
  };

  struct ctor : public generic_fun
  {
    ctor(const std::string& name, const std::string& header_path="") :
      generic_fun(":ctor", name, header_path) {}
  };

  void
  include(const std::string& file)
  {
    function_loader.include(file);
  }

  void
  include_dir(const std::string& dir)
  {
    function_loader.include_dir(dir);
  }

} // end of namespace dyn
