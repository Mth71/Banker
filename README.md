# Algoritmo do Banqueiro (Implementação 5)

Implementação em C do algoritmo do Banqueiro (Silberschatz, *Operating System Concepts*, 10ª ed.), com número de clientes e de recursos definidos pela entrada. Disciplina de Infraestrutura de Software, CESAR School.

## Sistema operacional

Desenvolvido e testado no **Windows 11**, compilando e executando no **WSL** (Ubuntu 26.04.1 LTS), com `gcc` e `make` (pacote `build-essential`).

## Arquivos

| Arquivo | Descrição |
|---|---|
| `include/banker.h` | `struct banker` (nº de clientes e de recursos, vetor `available` e matrizes `maximum`, `allocation` e `need`) e protótipos das funções. |
| `src/main.c` | Lê e valida os argumentos, carrega o `customer.txt`, valida o `commands.txt` inteiro e executa os comandos, gerando o `result.txt`. |
| `src/input.c` | Leitura e validação dos arquivos de entrada (`customer.txt` e `commands.txt`), com mensagens de erro indicando arquivo e linha. |
| `src/banker.c` | Alocação e liberação da memória, teste de segurança, pedido (RQ), liberação (RL) e impressão do estado (`*`). |
| `Makefile` | Compila (`make`), testa (`make test`) e limpa (`make clean`). |

## Entrada e saída

- **Argumentos**: número de instâncias de cada tipo de recurso. Ex.: `./banker 10 5 7` (3 tipos de recurso).
- **`customer.txt`**: uma linha por cliente, com o máximo de cada recurso separado por vírgula (ex.: `7,5,3`).
- **`commands.txt`**: um comando por linha: `RQ <cliente> <recursos...>`, `RL <cliente> <recursos...>` ou `*`.
- **`result.txt`**: gerado pelo programa com o resultado de cada comando.

Os dois arquivos de entrada devem estar na pasta onde o programa é executado.

## Como compilar

```bash
make clean
make
```

Gera o executável `banker`.

## Como executar

```bash
./banker 10 5 7
cat result.txt
```

## Como testar

Com o `result.txt` do exemplo do enunciado salvo como `result_esperado.txt` na mesma pasta:

```bash
make test
```

Executa `./banker 10 5 7` e compara a saída com `diff`. Se não imprimir diferenças, a saída está idêntica à esperada.

Casos de erro (o programa mostra uma mensagem e encerra sem gerar o `result.txt`):

```bash
./banker              # sem argumentos
./banker 10 x 7       # argumento que não é número
./banker 10 5         # quantidade de argumentos diferente da dos arquivos
```

Também são tratados: `customer.txt` ou `commands.txt` ausente, e linhas em formato inválido (valor não numérico, quantidade de recursos errada, comando desconhecido, cliente inexistente).

## Como limpar

```bash
make clean
```

Remove o executável `banker` e o `result.txt`.