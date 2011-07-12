#ifndef MLN_DATA_STYLE
#define MLN_DATA_STYLE
#include <stdlib.h>
namespace mymln
{
  namespace data
  {
    std::string itoa(int value)
    {
      std::string output = "";
      if(value < 0){output+="-"; value = -value;}
      while(value >= 10)
      {
	output = (char)('0' + (value % 10)) + output;
	value /= 10;
      }
      output = (char)('0' + value) + output;
      return output;
    }
    
    enum text_align
    {
      Left,
      Right,
      Center,
      Justify,
      Justify_Left,
      Justify_Right,
      Justify_Center
    };
    
    /// The class style is use to describe the style of a paragraph or a title
    class style
    {
      public:
	style()
	{
	  Font_ = "arial";
	  Font_Size_ = 12;
	  Align_ = Left;
	}
	void set_font_size(int px)
	{
	  Font_Size_ = px;
	}
	int get_font_size()
	{
	  return Font_Size_;
	}
	void set_text_align(text_align align)
	{Align_ = align;}
	text_align get_text_align()
	{return Align_;}
	std::string get_text_align_string()
	{return align_to_string_(Align_);}
	// WARNING THIS FUNCTION USE iota
	// iota is define only in mymln::data
	// this is not a standart function you can use everywhere
	std::string To_HTML_Style(int ID, int pmin_X, int pmin_Y)
	{
	  std::string output = "st" + itoa(ID) + "{";
	  output += "font-family:" + Font_  + ";";
	  output += "font-size:" + itoa(Font_Size_) + "px;";
	  output += "text-align:" + align_to_string_(Align_) + ";";
	  output += "position:absolute;";
	  output += "left:" + itoa(pmin_X) + ";";
	  output += "top:" + itoa(pmin_Y) + ";";
	  output += "} \n";
	  // FIXME
	  return output;
	}
      private:
	
		std::string Font_;
	unsigned int Font_Size_;
	text_align Align_;
	
	std::string align_to_string_(text_align align)
	{
	  switch(align)
	  {
	    case Left: return "left"; 
	    case Right: return "right"; 
	    case Center: return "center"; 
	    case Justify: return "justify"; 
	    case Justify_Left: return "justify"; 
	    case Justify_Right: return "justify";
	    case Justify_Center: return "justify";
	      
	  }
	}

    };
  }
}
#endif