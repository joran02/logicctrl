rm -rf makefile.rule
cp -rf makefile_imx385.rule makefile.rule
cd libsrc && rm -f libtw.so && make && cd ..
cd app && rm -f hi_35xx_cam && make && cd ..

cp lib/*so /nfs/imx385
cp app/hi_35xx_cam /nfs/imx385

