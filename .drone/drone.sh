#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -e
export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export PATH=~/.local/bin:/usr/local/bin:$PATH

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone -b $BOOST_BRANCH https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule init libs/align
git submodule init libs/assert
git submodule init libs/atomic
git submodule init libs/config
git submodule init libs/container_hash
git submodule init libs/core
git submodule init libs/move
git submodule init libs/predef
git submodule init libs/static_assert
git submodule init libs/throw_exception
git submodule init libs/type_traits
git submodule init libs/detail
git submodule init libs/integer
git submodule init tools/build
git submodule init libs/headers
git submodule init tools/boost_install
git submodule init tools/cmake
git submodule init libs/preprocessor
git submodule init libs/bind
git submodule update
cp -r $TRAVIS_BUILD_DIR/* libs/smart_ptr
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

echo "using $TOOLSET : : $COMPILER ;" > ~/user-config.jam
./b2 -j3 libs/smart_ptr/test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release ${UBSAN:+cxxflags=-fsanitize=undefined cxxflags=-fno-sanitize-recover=undefined linkflags=-fsanitize=undefined debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}

elif [ "$DRONE_JOB_BUILDTYPE" == "e15dfeee58-eaeb1fd44b" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone -b $BOOST_BRANCH https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule init libs/align
git submodule init libs/assert
git submodule init libs/atomic
git submodule init libs/config
git submodule init libs/container_hash
git submodule init libs/core
git submodule init libs/move
git submodule init libs/predef
git submodule init libs/static_assert
git submodule init libs/throw_exception
git submodule init libs/type_traits
git submodule init libs/detail
git submodule init libs/integer
git submodule init tools/build
git submodule init libs/headers
git submodule init tools/boost_install
git submodule init tools/cmake
git submodule init libs/preprocessor
git submodule init libs/bind
git submodule update
cp -r $TRAVIS_BUILD_DIR/* libs/smart_ptr
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

mkdir __build__ && cd __build__
cmake -DBOOST_ENABLE_CMAKE=1 -DBoost_VERBOSE=1 -DBOOST_INCLUDE_LIBRARIES=smart_ptr -DBUILD_TESTING=ON ..
ctest --output-on-failure -R boost_smart_ptr

elif [ "$DRONE_JOB_BUILDTYPE" == "e15dfeee58-11b2ac3185" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone -b $BOOST_BRANCH https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule init libs/align
git submodule init libs/assert
git submodule init libs/atomic
git submodule init libs/config
git submodule init libs/container_hash
git submodule init libs/core
git submodule init libs/move
git submodule init libs/predef
git submodule init libs/static_assert
git submodule init libs/throw_exception
git submodule init libs/type_traits
git submodule init libs/detail
git submodule init libs/integer
git submodule init tools/build
git submodule init libs/headers
git submodule init tools/boost_install
git submodule init tools/cmake
git submodule init libs/preprocessor
git submodule init libs/bind
git submodule update
cp -r $TRAVIS_BUILD_DIR/* libs/smart_ptr
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

cd libs/smart_ptr/test/cmake_subdir_test && mkdir __build__ && cd __build__
cmake ..
cmake --build .
cmake --build . --target check

elif [ "$DRONE_JOB_BUILDTYPE" == "e15dfeee58-49b3a6b0b2" ]; then

echo '==================================> INSTALL'

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone -b $BOOST_BRANCH https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule init libs/align
git submodule init libs/assert
git submodule init libs/atomic
git submodule init libs/config
git submodule init libs/container_hash
git submodule init libs/core
git submodule init libs/move
git submodule init libs/predef
git submodule init libs/static_assert
git submodule init libs/throw_exception
git submodule init libs/type_traits
git submodule init libs/detail
git submodule init libs/integer
git submodule init tools/build
git submodule init libs/headers
git submodule init tools/boost_install
git submodule init tools/cmake
git submodule init libs/preprocessor
git submodule init libs/bind
git submodule update
cp -r $TRAVIS_BUILD_DIR/* libs/smart_ptr
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

pip install --user cmake
mkdir __build__ && cd __build__
cmake -DBOOST_ENABLE_CMAKE=1 -DBoost_VERBOSE=1 -DBOOST_INCLUDE_LIBRARIES=smart_ptr -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=~/.local ..
cmake --build . --target install
cd ../libs/smart_ptr/test/cmake_install_test && mkdir __build__ && cd __build__
cmake -DCMAKE_INSTALL_PREFIX=~/.local ..
cmake --build .
cmake --build . --target check

fi
