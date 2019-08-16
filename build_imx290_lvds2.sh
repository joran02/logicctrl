rm -rf makefile.rule
cp -rf makefile_imx290_lvds2.rule makefile.rule
cd libsrc && rm -f libtw.so && make && cp libtw.so ../lib&& cd ..
cd app && rm -f hi_35xx_cam && make && cd ..
pwd
cp app/hi_35xx_cam /nfs/imx290_lvds
cp lib/*.so /nfs/imx290_lvds

