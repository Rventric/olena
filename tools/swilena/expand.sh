#! /bin/sh

[ -d "$1" -a -f "$1/expand.sh" -a -f "$1/ChangeLog" ] || \
  { echo "usage: $0 <swilena srcdir>" >&2; exit 1; }

SWILENA=`cd $1 && pwd`

mkdir -p "$SWILENA/src"
mkdir -p "$SWILENA/python"

MODULES="$MODULES ntg ntg_cplx ntg_int_u ntg_int_s"
for dim in 1 2 3; do

  ## {1d,2d,3d} families
  for mod in point structelt w_win imagesize image; do
     cat >"$SWILENA/src/swilena_$mod${dim}d.i" <<EOF
%include swilena_$mod.i
%module swilena_$mod${dim}d
decl_$mod($dim)
EOF
     MODULES="$MODULES ${mod}${dim}d"
  done

  ## w_window instances
  for t in int float; do
     echo "make_w_win(w_win${dim}d_$t, $dim, $t)" >>"$SWILENA/src/swilena_w_win${dim}d.i"
  done

  ## Image instances
  cat >>"$SWILENA/src/swilena_image${dim}d.i" <<EOF
%import swilena_ntg.i
%import swilena_ntg_int_u.i
%import swilena_ntg_int_s.i
EOF
  for t in bin \
    int_u8 int_u16 int_u32 \
    int_s8 int_s16 int_s32 \
    int_u8s int_u16s int_u32s \
    int_s8s int_s16s int_s32s \
    float_s float_d # rgb_8 rgb_16 rgb_32
    do
    short=`echo $t | sed -e 's,int_,,g;s,_,,g'`
    echo "make_image(image${dim}d_$short, $dim, oln::$t)" >> "$SWILENA/src/swilena_image${dim}d.i"
  done
done

#################### Python stuff #######################

PYMODS=''
PYTHON=''

header_python()
{
cat <<EOF
## Process this file through Automake to produce Makefile.in -*- Makefile -*-
##
## Makefile.am for swilena/python
## NOTE: this file was generated automatically by expand.sh
##

INCLUDES = -I/usr/include/python2.2 -I\$(srcdir)/../src 
AM_CPPFLAGS = -DOLN_EXCEPTIONS 
# AM_CXXFLAGS = \$(CXXFLAGS_OPTIMIZE) 
AM_LDFLAGS = -shared -lswigpy 
 
EOF
}

dump_python()
{
    echo -n "pyexec_PROGRAMS ="
    ilist=0
    for mod in $MODULES; do
      if [ `expr $ilist % 4` = 0 ]; then
         echo " \\"; echo -ne "\t"
      fi
      echo -n " _swilena_$mod.so" 
      ilist=`expr $ilist + 1`
    done
    echo; echo
    echo -n "python_PYTHON ="
    ilist=0
    for mod in $MODULES; do
      if [ `expr $ilist % 4` = 0 ]; then 
         echo " \\"; echo -ne "\t" 
      fi 
      echo -n " swilena_$mod.py"
      ilist=`expr $ilist + 1` 
    done
    echo; echo
    for mod in $MODULES; do
      echo "_swilena_${mod}_so_SOURCES = swilena_${mod}_wrap.cxx"
    done
    echo
    ilist=0
    for mod in $MODULES; do
      if [ -r "$SWILENA/src/swilena_${mod}.i" ]; then 
         sdir=src
      else
         sdir=meta
      fi
      echo "swilena_${mod}_wrap.cxx swilena_${mod}.py: \$(srcdir)/../$sdir/swilena_${mod}.i"
      echo -e "\t\$(SWIG) -c -c++ -python -I\$(srcdir)/../src -I\$(srcdir)/../meta \$(CPPFLAGS) -o swilena_${mod}_wrap.cxx \$(srcdir)/../$sdir/swilena_${mod}.i"
      echo
    done
}

############ Generic stuff ###########

header_src() {
    cat <<EOF
## Process this file through Automake to produce Makefile.in -*- Makefile -*- 
## 
## Makefile.am for swilena/src
## NOTE: this file was generated automatically by expand.sh 
## 
EOF
}

dump_src() {
    echo -n "EXTRA_DIST ="
    ilist=0
    for mod in $MODULES; do
      if [ -r "$SWILENA/src/swilena_${mod}.i" ]; then 
         if [ `expr $ilist % 4` = 0 ]; then
	    echo " \\"
            echo -ne "\t"
	 fi
	 echo -n " swilena_${mod}.i"
	 ilist=`expr $ilist + 1`
      fi
    done
    echo
}

############ Output ###############


header_src >"$SWILENA/src/Makefile.am"
dump_src >>"$SWILENA/src/Makefile.am"
header_python >"$SWILENA/python/Makefile.am"
dump_python >>"$SWILENA/python/Makefile.am"
