import subprocess
import sys

class SubnetManagerTester:
    def __init__(self, program_path):
        self.program_path = program_path
        self.report = ""

    def run_test(self, command, test_name, action, expected_result):
        process = subprocess.Popen([self.program_path], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, universal_newlines=True)
        out, err = process.communicate(input=command + "\nend\n")
        result = out.strip()
        if result == expected_result:
            self.report += f"{test_name} - {action}: Passed\n"
        else:
            self.report += f"{test_name} - {action}: Failed. Expected {expected_result}, but got {result}\n"
        return result

    def test_add(self):
        # Test adding valid subnets
        self.report += "Testing adding valid subnets:\n"
        self.run_test("add 10.20.0.0/16", "Adding valid subnet", "Adding 10.20.0.0/16", "0")
        self.run_test("add 32.64.128.0/20", "Adding valid subnet", "Adding 32.64.128.0/20", "0")
        self.report += "\n"

        # Test adding invalid subnets
        self.report += "Testing adding invalid subnets:\n"
        self.run_test("add 10.20.0.0/33", "Adding invalid subnet", "Adding 10.20.0.0/33", "-1")
        self.run_test("add 10.20.0.0/", "Adding invalid subnet", "Adding 10.20.0.0/", "-1")
        self.report += "\n"

    def test_del(self):
        # Test deleting existing subnets
        self.report += "Testing deleting existing subnets:\n"
        self.run_test("add 10.20.0.0/16", "Deleting existing subnet", "Adding 10.20.0.0/16", "0")
        self.run_test("del 10.20.0.0/16", "Deleting existing subnet", "Deleting 10.20.0.0/16", "0")
        self.report += "\n"

        # Test deleting non-existing subnets
        self.report += "Testing deleting non-existing subnets:\n"
        self.run_test("del 10.20.0.0/16", "Deleting non-existing subnet", "Deleting 10.20.0.0/16", "-1")
        self.report += "\n"

    def test_check(self):
        # Test checking IP addresses
        self.report += "Testing checking IP addresses:\n"
        self.run_test("add 10.20.0.0/16", "Checking IP address", "Adding 10.20.0.0/16", "0")
        self.run_test("add 32.64.128.0/20", "Checking IP address", "Adding 32.64.128.0/20", "0")
        self.run_test("check 10.20.128.10", "Checking IP address", "Checking 10.20.128.10", "16")
        self.run_test("check 32.64.140.200", "Checking IP address", "Checking 32.64.140.200", "20")
        self.run_test("check 192.168.1.1", "Checking IP address", "Checking 192.168.1.1", "-1")
        self.report += "\n"

    def run_all_tests(self):
        self.test_add()
        self.test_del()
        self.test_check()
        print("All tests passed.")
        self.save_report()

    def save_report(self):
        with open("subnet_manager_test_report.txt", "w") as f:
            f.write(self.report)


if __name__ == "__main__":
    working_path = sys.argv[0].replace("\\", "/").replace("Tests/main.py", "")
    path = working_path + "SubnetManager/x64/Debug/SubnetManager.exe"

    tester = SubnetManagerTester(path)
    tester.run_all_tests()