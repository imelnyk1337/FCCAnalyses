echo "Setting up key4hep ..."
. /cvmfs/sw.hsf.org/key4hep/setup.sh
echo "Setting up special ROOT for profiling ..."
. /opt/root/v6-26-06/bin/thisroot.sh
echo "Setting up FCCAnalyses built for profiling ..."
#_fccana="/opt/FCCAnalyses/v0.6.0"
_fccana="/home/imelnyk/FCCAnalyses/install"
export PATH=${_fccana}/bin:$PATH
export LD_LIBRARY_PATH=${_fccana}/lib:${_fccana}/lib64:$LD_LIBRARY_PATH
export PYTHONPATH=${_fccana}/python:$PYTHONPATH
export ROOT_INCLUDE_PATH=${_fccana}/include:$ROOT_INCLUDE_PATH
echo "Setting up FlameGraph ..."
export PATH=/opt/FlameGraph:$PATH
