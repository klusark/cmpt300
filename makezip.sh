mkdir zip
cp *.cpp zip/
cp *.h zip/
cp makefile zip/
cd zip
zip -D -m -9 CMPT300-Project2 *
mv CMPT300-Project2.zip ../CMPT300-Project2.zip
