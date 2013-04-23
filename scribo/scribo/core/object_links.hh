// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef SCRIBO_CORE_OBJECT_LINKS_HH
# define SCRIBO_CORE_OBJECT_LINKS_HH

/// \file
///
/// \brief Object links representation.


# include <mln/util/array.hh>
# include <mln/util/tracked_ptr.hh>

# include <scribo/core/component_set.hh>

# include <scribo/core/concept/serializable.hh>

namespace scribo
{

  using namespace mln;

  // Forward declaration.
  template <typename L> class object_links;


  namespace internal
  {

    /// Data structure for \c scribo::object_links<I>.
    template <typename L>
    struct object_links_data
    {
      object_links_data();
      object_links_data(const component_set<L>& components, unsigned size);
      object_links_data(const component_set<L>& components,
			unsigned size, unsigned default_link_id);

      mln::util::array<unsigned> comp_to_link_;
      component_set<L> components_;
    };

  } // end of namespace scribo::internal




  /*! \brief Object links representation.

    This structure is meant to store link information between
    components. Linking components can be considered as a first step
    towards component grouping.

    It requires a component_set to be constructed. Each component
    existing in the component_set may have link in an object_link
    structure. If no component_set is used for construction, this
    object is invalid (\sa is_valid()).



    \ingroup grpstruct
  */
  template <typename L>
  class object_links : public Serializable<object_links<L> >
  {
    typedef internal::object_links_data<L> data_t;

  public:
    /// Default constructor. It produces an invalid structure.
    object_links();

    /// Construct a valid object_links. Links is enabled for each
    /// valid component but no link is set. Invalid components links
    /// are disabled.
    object_links(const component_set<L>& components);

    /// Construct a valid object_links. Links is enabled for each
    /// valid component and set by default towards component with id
    /// \p default_link_id. Invalid components links are disabled.
    object_links(const component_set<L>& components, unsigned default_link_id);

    /// Return the underlying component_set.
    const component_set<L>& components() const;

    /// Return True if this object_links structure is correctly
    /// constructed.
    bool is_valid() const;

    /// Return True if component \p comp_id can be linked to another
    /// component.
    bool has_linking_enabled(unsigned comp_id) const;

    /// Return True if component \p comp_id has a link starting from
    /// itself to another component.
    bool is_linked(unsigned comp_id) const;

    /// Return the number of links. This is equivalent to the number
    /// of components + the background.
    unsigned nelements() const;

    /// Link related methods.
    /// \{
    /// Set link between component \p from_id and \p to_id.
    void update(unsigned from_id, unsigned to_id);

    /// Reset link for component with id \p id. This component can be
    /// linked later.
    void clear(unsigned id);

    /// Do not allow component with id \p id to be linked to another
    /// component.
    void disable_linking(unsigned id);

    /// Get link id for component \p comp_id.
    const unsigned& operator()(unsigned comp_id) const;
    /// \}

    /// Returns the underlying array encoding the component
    /// links. Indexes in array correspond to component ids and the
    /// corresponding value is the component id involved in the link.
    const mln::util::array<unsigned>& comp_to_link() const;

    /// Initialize links. Each component is linked to itself (i.e. has
    /// no link). Invalid components have linking disabled.
    void init();

    /// Make a deep copy of this structure.
    object_links<L> duplicate() const;

  private:
    mln::util::tracked_ptr<data_t> data_;
  };


  /// \relates object_links
  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_links<L>& links);


# ifndef MLN_INCLUDE_ONLY


  namespace internal
  {


    /// Data structure for \c scribo::object_links<I>.
    template <typename L>
    object_links_data<L>::object_links_data()
    {
    }


    template <typename L>
    object_links_data<L>::object_links_data(const component_set<L>& components,
					    unsigned size)
      : comp_to_link_(size), components_(components)
    {
    };


    template <typename L>
    object_links_data<L>::object_links_data(const component_set<L>& components,
					    unsigned size, unsigned default_link_id)
      : comp_to_link_(size, default_link_id), components_(components)
    {
    };


  } // end of namespace scribo::internal



  template <typename L>
  object_links<L>::object_links()
  {
    data_ = new data_t();
  }


  template <typename L>
  object_links<L>::object_links(const component_set<L>& components)
  {
    data_ = new data_t(components, value::next(components.nelements()));

    // Initialize with no link.
    init();
  }


  template <typename L>
  object_links<L>::object_links(const component_set<L>& components,
				unsigned default_link_id)
  {
    data_ = new data_t(components, value::next(components.nelements()),
		       default_link_id);
  }


  template <typename L>
  const component_set<L>&
  object_links<L>::components() const
  {
    return data_->components_;
  }


  template <typename L>
  bool
  object_links<L>::is_valid() const
  {
    return data_->components_.is_valid()
      && data_->components_.nelements() == (this->nelements() - 1);
  }


  template <typename L>
  bool
  object_links<L>::has_linking_enabled(unsigned comp_id) const
  {
    mln_precondition(is_valid());
    mln_precondition(comp_id < data_->comp_to_link_.nelements());

    return data_->comp_to_link_(comp_id) != 0;
  }

  template <typename L>
  bool
  object_links<L>::is_linked(unsigned comp_id) const
  {
    mln_precondition(is_valid());
    mln_precondition(comp_id < data_->comp_to_link_.nelements());

    return has_linking_enabled(comp_id)
      && data_->comp_to_link_(comp_id) != comp_id;
  }


  template <typename L>
  unsigned
  object_links<L>::nelements() const
  {
    return data_->comp_to_link_.nelements();
  }


  template <typename L>
  const unsigned&
  object_links<L>::operator()(unsigned comp_id) const
  {
    return data_->comp_to_link_(comp_id);
  }

  template <typename L>
  void
  object_links<L>::update(unsigned from_id, unsigned to_id)
  {
    data_->comp_to_link_(from_id) = to_id;
  }

  template <typename L>
  void
  object_links<L>::clear(unsigned id)
  {
    data_->comp_to_link_(id) = id;
  }

  template <typename L>
  void
  object_links<L>::disable_linking(unsigned id)
  {
    data_->comp_to_link_(id) = 0;
  }

  template <typename L>
  const mln::util::array<unsigned>&
  object_links<L>::comp_to_link() const
  {
    return data_->comp_to_link_;
  }

  template <typename L>
  void
  object_links<L>::init()
  {
    for (unsigned i = 0; i < nelements(); ++i)
      if (data_->components_(i).tag() == component::Ignored)
	disable_linking(i);
      else
	clear(i);
  }

  template <typename L>
  inline
  object_links<L>
  object_links<L>::duplicate() const
  {
    object_links<L> output;
    output.data_ = new data_t();

    *(output.data_.ptr_) = *(data_.ptr_);
    return output;
  }


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_links<L>& links)
  {
    ostr << "object_links[";

    for_all_links(l, links)
      ostr << l << "->" << links.comp_to_link()[l] << ", ";

    ostr << "]";

    return ostr;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_OBJECT_LINKS_HH
