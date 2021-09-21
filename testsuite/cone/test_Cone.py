import os
import pytest
import oftest
from oftest import run_reset_case


@pytest.fixture(scope='class') # descruct all tests
def load_errorfiles():
    dir_name = os.path.dirname(os.path.abspath(__file__))
    # dir_name = oftest.base_dir()
    name = os.path.join(dir_name,"postProcessing/avgT/0/writeAvg.dat")
    avgT = oftest.read_functionObject(name)
    return avgT


class TestCone:

    def test_completed(self,run_reset_case):
        log = oftest.path_log()
        assert oftest.case_status(log) == 'completed' # checks if run completes

    def test_temp(self,run_reset_case,load_errorfiles):
        avgT = load_errorfiles
        finalT =  abs(avgT.iloc[-1,1])
        assert  finalT == pytest.approx(600,1e-2)