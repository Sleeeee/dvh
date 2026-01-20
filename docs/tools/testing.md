# Testing the project

This document's purpose is to explain the testing mechanisms that are set up in the project and their integration in the CI/CD pipeline.

## Unit tests

[Unity](https://www.throwtheswitch.org/unity) is a unit testing framework for C, with a focus on embedded software. One of its advantages is its high portability, you only need to import three files into the project to get started. They are located in `firmare/tests/unit/lib/`.

The code for DVH includes Unity tests to ensure a decent quality and functionality. It is important to note that the DVH board runs on an ARM architecture, while the unit tests are compiled and run on an AMD architecture. This is perfectly fine, because the goal of those tests is only to validate the logic flow, and not to emulate the firmware running on the board.

Since each code module is meant to have its own Unity script, a bash script has been written to automate the testing process. It should be run from the `firmware/` directory, and provides a similar output :

```
$ ./tests/unit/run_tests.sh
Running unit tests...
Compiling tests/unit/src/test_lab_swd.c...
Binary compiled successfully
Running tests on tests/unit/src/test_lab_swd.c...
tests/unit/src/test_lab_swd.c:39:test_Lab_SWD_Solve_Flag_One_should_copy:PASS
tests/unit/src/test_lab_swd.c:40:test_Lab_SWD_Solve_Flag_Two_should_decrypt:PASS
tests/unit/src/test_lab_swd.c:41:test_Lab_SWD_Solve_Flag_Three_should_decrypt:PASS

-----------------------
3 Tests 0 Failures 0 Ignored
OK
Cleaning up binary...
Test results: 1 total, 1 passed
Testing successful
```

## Renode tests

For more complex, integrated system testing, [Renode](https://renode.readthedocs.io/en/latest/) allows us to emulate the board and manipulate the hardware to perform thorough testing, including reading register values, UART output, and so much more. To automate the whole process by running commands in the CLI, we use the [Robot Framework](https://docs.robotframework.org/docs) in dedicated `.robot` script files.

Because Renode does not simulate a complex hardware setup, we need to build appropriate firmware for the environment, which has been implemented with the `DSIMULATION` flag :

```
cmake -B build_simulation -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake -DSIMULATION=ON
```

The Renode testing workflow has also been automated in its own bash script, which should be run from the `firmware/` directory : 

```
$ ./tests/renode/run_tests.sh
Running Renode tests...
Preparing suites
Started Renode instance on port 49152; pid 13
Starting suites
Running tests/renode/src/test_lab_swd.robot
+++++ Starting test 'test_lab_swd.Verify Lab SWD'
+++++ Finished test 'test_lab_swd.Verify Lab SWD' in 1.73 seconds with status OK
Suite tests/renode/src/test_lab_swd.robot finished successfully in 2.01 seconds.
Cleaning up suites
Closing Renode pid 13
Aggregating all robot results
Output:  /project/build_simulation/tests/robot_output.xml
Log:     /project/build_simulation/tests/log.html
Report:  /project/build_simulation/tests/report.html
Tests finished successfully :)
Testing successful
```

> Please note that the script spins up a Docker container to perform the tests.

## Automated testing

The testing methods mentioned above have all been integrated in GitHub Actions as part of the CI/CD pipeline. They are included in the `.github/workflows/test.yaml` workflow, which runs separate jobs and is triggered on every push and pull request on the `main` branch.
