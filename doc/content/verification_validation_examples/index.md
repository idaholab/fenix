# FENIX Verification, Validation, and Example Cases

For [software quality assurance (SQA)](sqa/index.md exact=True) purposes, FENIX undergoes verification, validation, and benchmarking. The FENIX development team defines these terms as:

- +Verification+: Comparing FENIX predictions against analytical solutions in different conditions, which are often simple cases.
- +Validation+: Comparing FENIX predictions against experimental data.
- +Benchmarking+: Comparing FENIX predictions against other codes.

Note that in addition to monitoring FENIX performance and reproducibility in verification and validation cases, the effects of changes made to FENIX are tracked. A series of automated tests are performed via continuous integration using [CIVET](https://civet.inl.gov/repo/1108) to help identify any changes in FENIX's predictions, therefore ensuring stability and robustness.

Example cases have also been added, independent of the SQA cases shown below, which showcase existing FENIX capabilities and demonstrate potential applications. In all cases, the existing input files should allow users to leverage prior work.

Finally, for more references of FENIX usage, a list of publications supporting FENIX development can be found [here](publications.md).

FENIX couples with other MOOSE-based applications, such as [TMAP8](https://mooseframework.inl.gov/tmap8) or [Cardinal](https://cardinal.cels.anl.gov/). These projects have their own documentation regarding SQA and examples, which can be found at the following locations:

- [TMAP8 Verification and Validation](https://mooseframework.inl.gov/tmap8/verification_and_validation)
- [Cardinal Tutorial](https://cardinal.cels.anl.gov/tutorials/index.html)

# List of verification cases

!alert construction title=Under development - no verification cases are available yet
FENIX is under active development and does not currently have any verification cases fully documented for users.
However, several verification cases are already available in the `fenix/test/tests` folder.

# List of validation cases

!alert construction title=Under development - no validation cases are available yet
FENIX is under active development and does not currently have any validation cases available to users.

# List of benchmarking cases

| Case    | Title                                                                              |
| ------- | ---------------------------------------------------------------------------------- |
| 1 | [Lieberman Ion Wall Losses](lieberman.md) |




# List of example cases

!alert construction title=Under development - no example cases are available yet
FENIX is under active development and does not currently have any example cases available to users.

