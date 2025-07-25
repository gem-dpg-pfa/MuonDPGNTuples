Create a dataset according to an HLT configuration starting from an EphemeralHLTPhysics dataset.

- Modify the input file in `hlt.sh` to point to another ephemeral run, then run the script:

```bash
./hlt.sh [output-directory]
```

- Check the content of `outputRPCMON.root` in the output directory.

- To compare it with an edited version of the HLT menu, edit a copy of the `hlt.py` file in the output directory and run it with `cmsRun`.
