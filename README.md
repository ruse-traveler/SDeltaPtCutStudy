# SDeltaPtCutStudy

This class reads in the output of the SVtxEvaluator class and studies how the percent-error on the reconstructed transverse momentum varies with certain track populations.

## Usage

After copying the source files (in `src/`), compile the package in the usual manner:

```
./autogen.sh --prefix=$MYINSTALL
make -j 4
make install
```

And then run with:

```
root -b -q DoDeltaPtCutStudy.C
```
