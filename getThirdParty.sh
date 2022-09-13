#!/bin/sh

juce_dir_name="./JUCE"
juce_commit_hash_target="2f980209c" #Tag 6.1.6
juce_commit_hash_target_full="2f980209cc4091a4490bb1bafc5d530f16834e58"
juce_git_url="https://github.com/juce-framework/JUCE.git"
juce_branch_name="develop"

project_dir=`pwd`

## validate thirdparty directory
if [ ! -d thirdparty ];
then
    mkdir thirdparty
fi

cd thirdparty

## download JUCE
if test -d $juce_dir_name
then
    echo "JUCE already exists"
    cd $juce_dir_name
    juce_commit_hash_current=`git log -1 --format="%h"`
    echo "Target JUCE version(commit hash) : $juce_commit_hash_target"
    echo "Current JUCE version(commit hash) : $juce_commit_hash_current"

else
    echo "JUCE not exists, installing JUCE . . ."
    git clone -b $juce_branch_name --single-branch $juce_git_url

    cd $juce_dir_name
fi

## change commit version of JUCE
if [ x$juce_commit_hash_current != x$juce_commit_hash_target ];
then
    echo "Change JUCE version"
    git reset --hard $juce_commit_hash_target_full
fi


## install JUCE globally on system

cmake -B cmake-build-install -DCMAKE_INSTALL_PREFIX=./install

cmake --build cmake-build-install --target install
