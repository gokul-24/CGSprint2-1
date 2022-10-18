#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "header.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CUnit/Automated.h"
#include "CUnit/Console.h"

//init suit - Initialize the suit 
int init_suite(void)
{
	return 0; // To Initialize the suite
}

//clean suit - deinitialize the suit 
int clean_suite(void)
{
	return 0; // To uninitialize the suite after Unit Testing
}


//sunny - valid input strings are passed to the checkvalidity function and checked if 1 is returned or not.
void test_SunnyCases(void) // Sunny Testcases
{
	char *str1 = "127.0.0.20";
	char *str2 = "127.34.213.1";
	char *str3 = "127.0.0.100";
	CU_ASSERT(check_BlackList(str1) == 0);
	CU_ASSERT(check_BlackList(str2) == 0);
	CU_ASSERT(check_BlackList(str3) == 0);
}

//rainy - invalid input strings are passed to the check_BlackList function and checked if 0 is returned or not.
void test_RainyCases(void) // Rainy Testcases
{
	
	char *str4 = "12.204.255.255";
	char *str5 = "127.0.0.10";
	char *str6 = "127.0.0.1606";
	CU_ASSERT(check_BlackList(str4) == 1);
	CU_ASSERT(check_BlackList(str5) == 1);
	CU_ASSERT(check_BlackList(str6) == 1);
}

int main(void) // Main function
{

	//creating pSuite1 variable for testing 
	CU_pSuite pSuite1 = NULL;

	// Initialize CUnit test registry
	if (CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	// Add suite1 to registry
	pSuite1 = CU_add_suite("Testing_Suite1", init_suite, clean_suite);
	if (pSuite1 == NULL)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	// add test1 to suite1
	if ((NULL == CU_add_test(pSuite1, "Testing Sunny Cases", test_SunnyCases)) ||
		(NULL == CU_add_test(pSuite1, "Testing Rainy Cases", test_RainyCases)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE); //Provides information about the test suites returned 
	CU_basic_run_tests(); // OUTPUT to the screen

	CU_cleanup_registry(); // Cleaning the Registry

	return CU_get_error();
}
