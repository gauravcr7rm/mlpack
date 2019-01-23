/**
 * @file gmm_train_test.cpp
 * @author Gaurav Tripathi
 *
 * Test mlpackMain() of gmm_train_main.cpp.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
 #include<string>


#define BINDING_TYPE BINDING_TYPE_TEST
static const std:: string testname = "gmm_train"


#include <mlpack/core.hpp>
#include <mlpack/core/util/mlpack_main.hpp>
#include "test_helper.hpp"
#include <mlpack/methods/gmm/gmm_train_main.cpp>
#include <mlpack/methods/kmeans/kmeans_main.cpp>


#include "no_constraint.hpp"
#include "diagonal_constraint.hpp"


#include <boost/test/unit_test.hpp>
#include "../test_tools.hpp"


using namespace mlpack;

struct GmmTrainTestFixture
{
public:
  GmmTrainTestFixture()
  {
    //cache in the options for this program
    CLI::RestoreSettings(testname);
  }


  ~GmmTrainTestFixture()
  {
    //clear the settings.
    CLI::ClearSettings();
  }
};

void ResetGmmTrainSetting()
{
  CLI::ClearSettings();
  CLI::RestoreSettings(testname);
}


BOOST_FIXTURE_TEST_SUITE(GmmTrainMainTest , GmmTrainTestFixture);


//To check if the gaussian is positive or not
BOOST_AUTO_TEST_CASE(GmmTrainValidGaussian)
{
  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");


  SetInputParam("input" , std::move(inputdata));
  SetInputParam("gaussian" , (int) -1); //invalid

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain(), std::runtime_error);
  Log::Fatal.ignoreInput = false;

}


/*To check if the number of gaussians in the output model is same as
that of input gaussian parameter or not*/
BOOST_AUTO_TEST_CASE(GmmTrainOutputModelGaussian)
{
   int g = 3;
  int trials = 2;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");

  SetInputParam("input" , std::move(inputdata));
  SetInputParam("gaussians" , g);
  SetInputParam("trials" , trials);
  
  MlpackMain();

  GMM* gmm = CLI::GetParam<GMM*>("output_model");
  BOOST_REQUIRE_EQUAL(gmm->gaussians() , g);
}


//Number of trials is provided or not
BOOST_AUTO_TEST_CASE(GmmTrainValidTrials)
{
  int g = 3;
  int trials = 0;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");


  SetInputParam("input" , std::move(inputdata));
  SetInputParam("gaussians" , g);
  SetInputParam("trials" , trials) //invalid

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain(), std::runtime_error);
  Log::Fatal.ignoreInput = false;
}


// if covariance flag is false then no_force_positive parameter is not specified
BOOST_AUTO_TEST_CASE(GmmTrainDiagonalCovarianceCheck)
{
  int g = 3;
  int t = 2;
  int n_f_p = 2;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");

  SetInputParam("input" , std::move(inputdata));
  SetInputParam("gaussians" , g);
  SetInputParam("trials" , t);
  SetInputParam("Diagonal_covariance" , false);
  SetinputParam("no_force_positive" , n_f_p); //invalid
  SetInputParam("noise" , (int) 0);

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain() , runtime_error);
  Log::Fatal.ignoreInput = false;

}


//max iterations must be positive
BOOST_AUTO_TEST_CASE(GmmTrainMaxIterations)
{
  int g = 3;
  int t = 2;
  int mi = -1;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");


  SetinputParam("input" , std::move(inputdata));
  SetinputParam("gaussians", g);
  SetinputParam("trials" , t);
  SetinputParam("max_iterations" ,mi)//invalid

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain() , runtime_error);
  Log::Fatal.ignoreInput = false;
}


//tolerence must be positive 
BOOST_AUTO_TEST_CASE(GmmTrainTolerence)
{
  int g = 3;
  int t = 2;
  int mi = 3;
  int tol = -1;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");

  SetinputParam("input" , std::move(inputdata));
  SetinputParam("gaussians", g);
  SetinputParam("trials" , t);
  SetinputParam("max_iterations" ,mi);
  SetinputParam("tolerence" , tol);//invalid

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain() , runtime_error);
  Log::Fatal.ignoreInput = false;

}


 /**
 * Checking that percentage is between 0 and 1 when --refined_start is specified
*/
BOOST_AUTO_TEST_CASE(RefinedStartPercentageTest)
{
  int g = 3;
  int t = 2;
  int mi = 3;
  int tol = -1;
  int c = 2;
  double P = 2.0;
  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");

  SetInputParam("input", std::move(inputData));
  SetInputParam("refined_start", true);
  SetInputParam("clusters", c);
  SetInputParam("percentage", std::move(P));     // Invalid

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain(), std::runtime_error);
  Log::Fatal.ignoreInput = false;
}


//Make sure that output model can be reused.
BOOST_AUTO_TEST_CASE(GmmTrainReuseGmmModel)
{
  int g = 3;
  int t = 2;
  int mi = 5;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");


  SetinputParam("input" , std::move(inputdata));
  SetinputParam("gaussians", g);
  SetinputParam("trials" , t);
  SetinputParam("max_iterations" ,mi)

  mlpakMain();
  
  GMM* gmm = CLI::GetParam<GMM*>("output_model");

  SetinputParam("input_model" , gmm);

  CLI::GetSingleton().Parameters()["gaussians"].wasPassed = false;
  CLI::GetSingleton().Parameters()["trials"].wasPassed = false;
  CLI::GetSingleton().Parameters()["max_iterations"].wasPassed = false;

  mlpackMain();

  GMM* gmm1 = CLI::GetParam<GMM*>("output_model");

  ApproximatelyEqual(gmm, gmm1);

}

//number of gaussians in the model trained from input model.
BOOST_AUTO_TEST_CASE(GmmTrainNumberOfGaussian)
{
  BOOST_AUTO_TEST_CASE(GmmTrainReuseGmmModel)
{
  int g = 3;
  int t = 2;
  int mi = 5;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");


  SetinputParam("input" , std::move(inputdata));
  SetinputParam("gaussians", g);
  SetinputParam("trials" , t);
  SetinputParam("max_iterations" ,mi)

  mlpakMain();
  
  GMM* gmm = CLI::GetParam<GMM*>("output_model");

  SetinputParam("input_model" , gmm);

  CLI::GetSingleton().Parameters()["gaussians"].wasPassed = false;
  CLI::GetSingleton().Parameters()["trials"].wasPassed = false;
  CLI::GetSingleton().Parameters()["max_iterations"].wasPassed = false;

  mlpackMain();

  GMM* gmm1 = CLI::GetParam<GMM*>("output_model");

  BOOST_REQUIRE_EQUAL(gmm1->gaussians(), g);

}

//checking that after noise addition matrix is invertible
BOOST_AUTO_TEST_CASE(GmmTrainNoiseAddition)
{
  int g = 3;
  int trials = 2;
  int maxi = 5;
  double noise = 0.68;

  arma::mat inputData;
  if (!data::Load("vc2.csv", inputData))
  BOOST_FAIL("Unable to load train dataset vc2.csv!");

  SetinputParam("input" , std::move(inputdata));
  SetinputParam("gaussians", g);
  SetinputParam("trials" , trials);
  SetinputParam("max_iterations" ,maxi);
  SetInputParam("noise" , noise);

  mlapckMain();

  GMM* gmm = CLI::GeParam<GMM*>("output_model");

  BOOST_REQUIRE_EQUAL(gmm->Diagonal_covariance(), true);
}

BOOST_AUTO_TEST_SUITE_END();
