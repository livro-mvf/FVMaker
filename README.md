# FVMaker

## Descrição
FVMaker é uma biblioteca em **C++20** para a solução numérica de equações diferenciais parciais utilizando o método dos volumes finitos (FVM). A biblioteca faz uso de recursos modernos do C++ para otimização e paralelização, garantindo desempenho elevado.

## Requisitos de Instalação
Para compilar e rodar a FVMaker, é necessário ter instalados os seguintes componentes:

- **Compilador C++ com suporte a C++20**  
  (ex.: GCC 10+, Clang 10+ ou MSVC 19.28+)

- **CMake 3.20 ou superior**

- **Intel TBB (Threading Building Blocks)**  
  A biblioteca depende do Intel TBB para paralelização.  
  No Ubuntu, por exemplo, instale com:
  ```sh
  sudo apt-get install libtbb-dev
  ```
  
> Caso o Intel TBB não esteja instalado, a configuração com CMake irá falhar com a mensagem:  
> "Intel TBB não encontrado. Certifique-se de que libtbb-dev está instalado."  
>  
> Essa exigência é definida no CMakeLists.txt citeturn0file0.

## Estrutura do Projeto
```
FVMaker/
├── FVMakerLib/          # Biblioteca principal
│   ├── include/         # Arquivos de cabeçalho da biblioteca
│   ├── src/             # Implementações da biblioteca
│   └── lib/             # Biblioteca compilada
├── Capitulos/           # Exemplos de programas
├── Tests/               # Conjunto de testes (opcional)
├── Sphinx/              # Documentação (opcional)
├── CMakeLists.txt       # Configuração do CMake
└── README.md            # Documentação geral do projeto
```

## Instruções de Compilação e Execução

1. **Clonar o repositório**
   ```sh
   git clone https://github.com/seu_usuario/FVMaker.git
   cd FVMaker
   ```

2. **Criar um diretório de compilação (fora da raiz do projeto)**
   ```sh
   mkdir build
   cd build
   ```

3. **Configurar o projeto com CMake**
   ```sh
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```
   *Observação:* Certifique-se de que o Intel TBB esteja instalado. Caso contrário, a configuração do CMake falhará.

4. **Compilar a biblioteca (e os exemplos/testes, se habilitados)**
   ```sh
   make -j$(nproc)
   ```

5. **Executar a biblioteca ou os programas de exemplo**
   Após a compilação, os binários estarão disponíveis no diretório configurado (por exemplo, em `FVMakerLib/lib` ou nos diretórios dos exemplos). Para executar um exemplo, por exemplo:
   ```sh
   ./Capitulos/Capitulo_1/programa_exemplo
   ```

6. **Executar os testes (opcional)**
   Se a opção `BUILD_TESTS` estiver ativada durante a configuração, você poderá executar os testes com:
   ```sh
   ctest
   ```

## Opções de Build
O CMakeLists.txt permite configurar as seguintes opções:

- **BUILD_TESTS:** Habilita a compilação dos testes (não pode ser usado simultaneamente com BUILD_EXAMPLES).
- **BUILD_EXAMPLES:** Habilita a compilação dos exemplos presentes na pasta `Capitulos/`.
- **BUILD_DOCUMENTATION:** Gera a documentação utilizando Doxygen e Sphinx (opcional).

> **Atenção:** Não é permitido habilitar as opções BUILD_TESTS e BUILD_EXAMPLES simultaneamente.  
>  
> As configurações de build e as otimizações (como -O3, -march=native, -flto e suporte a OpenMP) estão definidas no CMakeLists.txt citeturn0file0.

## Contribuição
Contribuições são bem-vindas! Para colaborar:

1. Faça um fork do repositório.
2. Crie um branch para sua funcionalidade:
   ```sh
   git checkout -b minha_feature
   ```
3. Realize suas alterações seguindo as boas práticas de desenvolvimento em C++.
4. Abra um Pull Request para revisão.

## Licença
FVMaker é distribuído sob a licença **BSD 3-Clause**. Consulte o arquivo [LICENSE.md](LICENSE.md) para o texto completo.