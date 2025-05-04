add_test([=[ImageProcessingTest.BlurSectionConversion]=]  /home/alfasamec3d/media/programs/Image_processor/build/testblursection [==[--gtest_filter=ImageProcessingTest.BlurSectionConversion]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ImageProcessingTest.BlurSectionConversion]=]  PROPERTIES WORKING_DIRECTORY /home/alfasamec3d/media/programs/Image_processor/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  testblursection_TESTS ImageProcessingTest.BlurSectionConversion)
