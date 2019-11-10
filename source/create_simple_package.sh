#!/bin/bash

# this script provides the simplest possible way to package molconv for a
# linux system. It creates a zip archive containing the compiled executable
# and the required chemkit libraries, along with a wrapper script to set
# the required paths

mkdir molconv

cp ../molconv_build/bin/molconv molconv/
strip molconv/molconv

cp /usr/local/lib/libchemkit.so molconv/
cp /usr/local/lib/libchemkit-io.so molconv/
cp /usr/local/lib/libchemkit-graphics.so molconv/
cp /usr/local/lib/libchemkit-gui.so molconv/

cat <<\EOF > molconv/start_molconv.sh
#!/bin/bash
export LD_LIBRARY_PATH=${PWD}:${LD_LIBRARY_PATH}
./molconv "$@"
EOF

cat <<\EOF > molconv/README
This package of molconv has been built and tested on Ubuntu 18.04 LTS.
To run molconv, you need to install the packages

 libqt5gui5
 libqt5opengl5
 libqt5xml5

Then just run ./start_molconv.sh
EOF

chmod +x molconv/start_molconv.sh

zip -r molconv.zip molconv/

