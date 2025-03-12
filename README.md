# Cache-Simulator
Um simulador de cache desenvolvido como a segunda atividade prática do curso de Arquitetura e Organização de Computadores II na Universidade Federal de Pelotas.

## Utilização do simulador
Compile os arquivos "Cache.cpp", "FileReader.cpp" e "main.cpp" em um arquivo e depois o execute.

Execute o seguinte comando para compilar o código:
   ```bash
   g++ main.cpp ./src/components/Cache.cpp ./src/utils/FileReader.cpp -o cache_simulator
```

## Execute 
Após compilar o projeto, execute o simulador com o seguinte comando:
```bash
  ./cache_simulator <nsets> <bsize> <assoc> <substPolicy> <flag> <filename>
  ```
  Parâmetros:

  - <nsets>: Número de conjuntos na cache.

  - <bsize>: Tamanho do bloco em bytes.

  - <assoc>: Grau de associatividade (número de vias por conjunto).

  - <substPolicy>: Política de substituição (R para Random, F para FIFO, L para LRU).

  - <flag>: Flag de saída (0 para formato livre, 1 para formato padrão).

  - <filename>: Nome do arquivo de entrada com os endereços de memória.

## Exemplo de uso:
```bash
  ./cache_simulator 256 4 1 R 1 bin_100.bin
  ```
  ## Estrutura do projeto
- '/src': Contém o código fonte do projeto.
- '/src/utils': Contém as funções auxiliares.
- '/src/components': Contém a lógica principal do simulador.
- '/include': Contém todos os headers e arquivos referenciados por outras pastas.
- '/test': Eventuais testes automatizados que podem ser adicionados.