rm -rf makefile.rule
cp -rf makefile_imx290_lvds.rule makefile.rule
cd libsrc && rm -f libtw.so && make && cd ..
cd app && rm -f hi_35xx_cam && make && cd ..
