#include <dimred/generate_data.h>
#include <iostream>
#include "../../../external/gtest-1.6.0/fused-src/gtest/gtest.h"
// MORE INFO HERE: http://code.google.com/p/googletest/wiki/AdvancedGuide

class DimRedTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        chdir(((String)(getXmippPath() + (String)"/resources/test")).c_str());
    }
};

TEST_F( DimRedTest, generate_data)
{
	GenerateData generator;

	// Swiss
	generator.generateNewDataset("swiss",1000,0);
	// generator.X.write("dimred/swiss.txt");
	// load swiss.txt; plot3(swiss(:,1),swiss(:,2),swiss(:,3),'.');
	Matrix2D<double> expectedX(1000,3);
	expectedX.read("dimred/swiss.txt");
	ASSERT_TRUE(expectedX.equal(generator.X,1e-5));

	// Helix
	generator.generateNewDataset("helix",1000,0);
	// generator.X.write("dimred/helix.txt");
	// load helix.txt; plot3(helix(:,1),helix(:,2),helix(:,3),'.');
	expectedX.read("dimred/helix.txt");
	ASSERT_TRUE(expectedX.equal(generator.X,1e-5));

	// Twin peaks
	generator.generateNewDataset("twinpeaks",1000,0);
	//generator.X.write("dimred/twinpeaks.txt");
	// load twinpeaks.txt; plot3(twinpeaks(:,1),twinpeaks(:,2),twinpeaks(:,3),'.');
	expectedX.resizeNoCopy(generator.X);
	expectedX.read("dimred/twinpeaks.txt");
	ASSERT_TRUE(expectedX.equal(generator.X,1e-5));
}

GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
