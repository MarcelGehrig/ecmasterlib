#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

build_dir="$script_dir/buildx86"
install_dir="$(dirname $script_dir)/installx86"


rm -rf "$build_dir"
echo rm -rf "$build_dir"


mkdir -p $build_dir
pushd $build_dir
echo cmake -DCMAKE_INSTALL_PREFIX="$install_dir" ..
cmake -DCMAKE_INSTALL_PREFIX="$install_dir" ..
make
make install
popd


#echo "copy EEROS to test folder"
#scp echo "copy to " "$target_name"
#scp $install_dir/lib/libeeros.so root@es107:/opt/eeros/lib/test
