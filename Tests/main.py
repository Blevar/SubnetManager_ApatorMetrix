import subprocess
import sys, os

# Proszę napisać skrypt w Pythonie wywołujący program napisany w C. Skrypt powinien kompleksowo testować opisane wyżej funkcjonalności. Należy przewidzieć rozszerzanie przypadków testowych o nowe scenariusze
# Skrypt powinien generować raport z przeprowadzonych testów.
#
# Autor: Jan Prusinowski
# Rev: 1

def run_program(args):
    try:
        result = subprocess.run(args, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        return e.stderr.strip()

def run_tests():
    working_path = sys.argv[0].replace("\\", "/").replace("Tests/main.py", "")
    path = working_path + "SubnetManager/x64/Debug/SubnetManager.exe"

    tests = [
        ("Add subnet 10.20.0.0/16", [path, "add", "10.20.0.0", "16"]),
        ("Add subnet 32.64.128.0/20", [path, "add", "32.64.128.0", "20"]),
        ("Check IP 10.20.128.10", [path, "check", "10.20.128.10"]),
        ("Check IP 32.64.139.200", [path, "check", "32.64.139.200"]),
        ("Check IP 192.168.1.1", [path, "check", "192.168.1.1"]),
        ("Delete subnet 10.20.0.0/16", [path, "del", "10.20.0.0", "16"]),
        ("Check IP 10.20.128.10 after deletion", [path, "check", "10.20.128.10"]),
        ("Add subnet with invalid mask", [path, "add", "10.20.0.0", "-1"]),
        ("Add subnet with invalid mask", [path, "add", "10.20.0.0", "33"]),
        ("Add subnet with out-of-range base IP", [path, "add", "256.0.0.0", "16"]),
        ("Delete non-existing subnet", [path, "del", "192.168.0.0", "24"])
    ]

    report = []

    for test_name, args in tests:
        report.append(f"Test: {test_name}")
        report.append(f"Command: {' '.join(args)}")
        report.append("Result:")
        report.append(run_program(args))
        report.append("-" * 50)

    return "\n".join(report)

if __name__ == "__main__":
    test_report = run_tests()
    with open("test_report.txt", "w") as f:
        f.write(test_report)
    print("Wygenerowano raport.")