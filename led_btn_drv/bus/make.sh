source mk.conf

make CROSS_COMPILE=${CROSS_COMPILE} ARCH=${ARCH} -j4 $1
