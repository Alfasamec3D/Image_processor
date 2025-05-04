add_test([=[ImageProcessingTest.GrayscaleConversion]=]  /mnt/9C468886468862BE/media/programs/Image_processor/build/testgrayscale [==[--gtest_filter=ImageProcessingTest.GrayscaleConversion]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ImageProcessingTest.GrayscaleConversion]=]  PROPERTIES WORKING_DIRECTORY /mnt/9C468886468862BE/media/programs/Image_processor/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  testgrayscale_TESTS ImageProcessingTest.GrayscaleConversion)
