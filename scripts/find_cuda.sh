if [ -d "/usr/local/cuda/-10.1/bin/" ]; then
    echo /usr/local/cuda/-10.1/bin/nvcc
else
    echo $(which nvcc)
fi
