import subprocess

def run_cache_simulator(command):
    # Executa o comando e captura a saída
    result = subprocess.run(command, stdout=subprocess.PIPE, text=True)
    return result.stdout.strip()

def parse_output(output):
    # Divide a saída em valores individuais
    values = list(map(float, output.split()))
    return values

def compare_results(actual, expected):
    # Compara os valores reais com os esperados
    if len(actual) != len(expected):
        return False
    for a, e in zip(actual, expected):
        if abs(a - e) > 0.0001:  # Considera uma pequena margem de erro
            return False
    return True

# Códigos ANSI para cores
GREEN = "\033[92m"  # Verde
RED = "\033[91m"    # Vermelho
RESET = "\033[0m"   # Resetar cor

# Exemplos de comandos e resultados esperados do PDF
test_cases = [
    {
        "command": ["./cache_simulator", "256", "4", "1", "R", "1", "bin_100.bin"],
        "expected": [100, 0.9200, 0.0800, 1.00, 0.00, 0.00]
    },
    {
        "command": ["./cache_simulator", "128", "2", "4", "R", "1", "bin_1000.bin"],
        "expected": [1000, 0.8640, 0.1360, 1.00, 0.00, 0.00]
    },
    {
        "command": ["./cache_simulator", "16", "2", "8", "R", "1", "bin_10000.bin"],
        "expected": [10000, 0.9298, 0.0702, 0.18, 0.79, 0.03]
    },
    {
        "command": ["./cache_simulator", "512", "8", "2", "R", "1", "vortex.in.sem.persons.bin"],
        "expected": [186676, 0.8782, 0.1218, 0.05, 0.93, 0.02]
    },
    {
        "command": ["./cache_simulator", "1", "4", "32", "R", "1", "vortex.in.sem.persons.bin"],
        "expected": [186676, 0.5440, 0.4560, 0.00, 1.00, 0.00]
    },
    {
        "command": ["./cache_simulator", "2", "1", "8", "R", "1", "bin_100.bin"],
        "expected": [100, 0.43, 0.57, 0.28, 0.68, 0.04]
    },
    {
        "command": ["./cache_simulator", "2", "1", "8", "L", "1", "bin_100.bin"],
        "expected": [100, 0.46, 0.54, 0.30, 0.67, 0.04]
    },
    {
        "command": ["./cache_simulator", "2", "1", "8", "F", "1", "bin_100.bin"],
        "expected": [100, 0.43, 0.57, 0.28, 0.68, 0.04]
    },
    {
        "command": ["./cache_simulator", "1", "4", "32", "L", "1", "vortex.in.sem.persons.bin"],
        "expected": [186676, 0.5756, 0.4244, 0.00, 1.00, 0.00]
    },
    {
        "command": ["./cache_simulator", "1", "4", "32", "F", "1", "vortex.in.sem.persons.bin"],
        "expected": [186676, 0.5530, 0.4470, 0.00, 1.00, 0.00]
    }
]

# Executa os testes
for i, test_case in enumerate(test_cases):
    print(f"Executando teste {i+1}...")
    output = run_cache_simulator(test_case["command"])
    actual_values = parse_output(output)
    
    # Exibe o esperado e o obtido
    print(f"Esperado: {test_case['expected']}")
    print(f"Obtido:   {actual_values}")
    
    # Compara os resultados e exibe a mensagem colorida
    if compare_results(actual_values, test_case["expected"]):
        print(GREEN + f"Teste {i+1} passou!" + RESET)
    else:
        print(RED + f"Teste {i+1} falhou!" + RESET)
    print()  # Linha em branco para separar os testes