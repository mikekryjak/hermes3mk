cmake . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/ssd_scratch/BOUT-next4may22/BOUT-dev/build" -DHERMES_BUILD_BOUT=False
cd build
make
cd ..
