#! /bin/csh -f

foreach f (*.gr)
/local/homes/nkrasno/PROJECTS/work/CMO/conv $f $f.E1.dat $f.E2.dat
end

