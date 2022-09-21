cmake . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/ssd_scratch/BOUT-snbfix/BOUT-dev/build" -DHERMES_BUILD_BOUT=False
cd build
make -j 8
cd ..
