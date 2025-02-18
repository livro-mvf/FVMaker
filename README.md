# FVMaker

## Descrição
FVMaker é uma biblioteca escrita em **C++20** para solução numérica de equações diferenciais parciais utilizando o **método dos volumes finitos (FVM)**. O projeto tem como objetivo fornecer uma implementação rápida, flexível e fácil de usar, sem dependências externas, utilizando apenas recursos modernos da **STL** e paralelização disponível no **C++20**.

## Estrutura do Projeto
```
FVMaker/                # Raiz do projeto
│── FVMakerLib/         # Biblioteca principal do projeto
│   │── include/        # Pasta de headers da biblioteca
│   │   │── FVMaker/    # Todos os arquivos de cabeçalho ficarão aqui
│   │── src/            # Implementações da biblioteca
│   │── lib/            # Biblioteca compilada (removida com make clean)
│── Capitulos/          # Programas relacionados aos capítulos do livro
│   │── Capitulo 1/     # Programa do Capítulo 1
│   │── Capitulo 2/     # Programa do Capítulo 2
│   │── ...             # Outras pastas para capítulos subsequentes
│── Tests/              # Testes da biblioteca usando Google Test (gtest)
│   │── Teste 1/        # Pasta de um grupo de testes
│   │   │── Subteste A/ # Teste específico dentro do grupo
│   │   │── Subteste B/ # Outro teste específico
│   │── Teste 2/        # Outro grupo de testes
│   │── ...             # Outras pastas de testes
│── Sphinx/             # Pasta para documentação do projeto
│── CMakeLists.txt      # Arquivo de configuração do CMake
│── README.md           # Arquivo de documentação geral do projeto
```

## Requisitos
- **Compilador compatível com C++20** (GCC 10+, Clang 10+, MSVC 19.28+)
- **CMake 3.20+**
- **Google Test (gtest)** para execução dos testes

## Compilação e Instalação

1. **Clonar o repositório**
   ```sh
   git clone https://github.com/seu_usuario/FVMaker.git
   cd FVMaker
   ```

2. **Criar um diretório de compilação e configurar o projeto**
   ```sh
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```

3. **Compilar a biblioteca e os programas**
   ```sh
   make -j$(nproc)
   ```

4. **Executar os testes** (se habilitados no CMake)
   ```sh
   ctest
   ```

## Opções de Compilação
O `CMakeLists.txt` está configurado para otimização total do desempenho, incluindo:
- **Compilação obrigatória em Release**, sem opção de Debug.
- **Uso de `-O3`, `-march=native`, `-flto` e outras otimizações**.
- **Compilação automática de programas em `Capitulos/` e `Tests/`**.
- **Geração de documentação com Sphinx e Doxygen** (opcional).
- **Obrigatoriedade de rodar o CMake dentro de uma pasta separada (`build/` ou outra)**.

## Como Compilar os Programas dos Capítulos
Os programas dentro da pasta `Capitulos/` serão compilados automaticamente. Para executar um programa específico, após a compilação, utilize:
   ```sh
   ./Capitulos/Capitulo_1/programa_exemplo
   ```
Substitua `Capitulo_1/programa_exemplo` pelo caminho correto do programa desejado.

## Como Executar os Testes
Se a opção `BUILD_TESTS=ON` for ativada no CMake, os testes serão compilados automaticamente. Para executá-los:
   ```sh
   make run_tests
   ```

## Contribuição
Contribuições são bem-vindas! Para contribuir:
1. Fork o repositório.
2. Crie um branch com a funcionalidade desejada (`git checkout -b minha_feature`).
3. Faça as alterações seguindo o [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
4. Abra um Pull Request para revisão.

## Licença
FVMaker é um software livre licenciado sob a **GNU General Public License v3.0**. Consulte o arquivo [LICENSE](LICENSE) para mais detalhes.
