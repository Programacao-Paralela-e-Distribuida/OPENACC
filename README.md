# Exemplos OpenACC
Nesse repositório encontramos os exemplos em OpenACC utilizados no livro. A seguir listamos os procedimentos necessários para a preparação do ambiente de execução para utilização do OpenACC.

## Preparação do Ambiente de Execução OpenACC 

### Introdução 
Antes de compilar qualquer código é importante saber quais dispositivos
aceleradores estão configurados para uso no sistema. Existem alguns
comandos que fornecem informações de modelos e características desses
dispositivos.

Para aceleradores da NVIDIA existes os comandos *nvidia-smi* e
*nvidia-settings*, que fornecem de informações de configuração como:
modelo, cpus, cuda core, memória. Com o compilador da PGI também existe
um comando chamado *pgaccelinfo* que fornece as principais
características dos aceleradores instalados no sistema.

``` {style="verbatim"}
$ nvidia-smi -q | grep "Product Name"
    Product Name                    : Quadro K420
    Product Name                    : Tesla K80
    Product Name                    : Tesla K80

$ pgaccelinfo | grep "Device Name"
    Device Name:                   Tesla K80
    Device Name:                   Tesla K80
    Device Name:                   Quadro K420
```

Os aceleradores da NVIDIA necessitam que sejam instalados os *drivers*
NVIDIA específicos para cada tipo de arquitetura e modelo. Conforme o
modelo do acelerador, o sistema identifica um dispositivo específico e
sua capacidade de computação conforme relação abaixo.

  | **Arquitetura**  | **Dispositivo** |  **capacidade de computação** |
  | ----------------- | ----------------- | ------------------------------ |
  |     Ampere      |       cc80          |           8.0                |
  |     Turing      |       cc75          |           7.5                |
  |      Volta      |       cc72          |           7.2                |
  |      Volta      |       cc70          |           7.0                |
  |     Pascal      |       cc62          |           6.2                |
  |     Pascal      |       cc60          |           6.0                |
  |     Maxwell     |       cc50          |           5.0                |
  |     Kepler      |       cc35          |           3.5                |
  |     Kepler     |        cc30          |           3.0                |

Quando o código é compilado ele identifica qual o *driver* NVIDIA está
instalado e gera o binário do código. O comando *nvidia-smi* fornece
informação da versão do *driver* NVIDIA instalado no sistema. O comando
*pgaccelinfo* da PGI fornece as informações do dispositivo de GPU
instalado.

``` {style="verbatim"}
$ nvidia-smi -q | grep "Driver Version"
Driver Version                      : 440.64.00

$ pgaccelinfo | grep "Default Target:"
Default Target:                cc35
```

### Compiladores

Para compilar os códigos feitos em OpenACC, gerando código para execução
em GPUs, é necessário o uso de compiladores que suportem OpenACC. A PGI
(Portland Group), tem uma versão disponibilizada para uso público sem a
necessidade de licença, também existe uma versão com uso de licença que
permite ter acesso à equipe de suporte da PGI.

A Cray também tem seu compilador o CCE 8.6.5, para usá-lo é necessário a
aquisição de uma licença, sendo que não existe um licença para uso
público.

O compilador GCC também possui suporte para o OpenACC conforme a seguir:

-   GCC 8: Suporta o OpenACC versão 2.0a

-   GCC 9: Suporta o OpenACC versão 2.5

-   GCC 10: Suporta o OpenACC versão 2.6

Os dispositivos suportados pelo GCC, no momento da escrita deste livro,
eram aqueles utilizando Nvidia PTX (nvptx) e AMD GCN (GPUs Fiji e Vega
GPUs).

Nos exemplos apresentados neste livro foi utilizado o compilador da PGI
disponibilizado para a comunidade, que pode ser baixado gratuitamente da
página da NVIDIA[^1].

#### Compilador PGI 

Para compilar códigos em C usaremos o comando *pgcc*, e para compilação
de códigos em C++ usar o comando *pgc++*. Algumas parâmetros básicos
devem ser usados durante a execução dos comandos *pgcc* e *pgc++*. Esses
parâmetros definem em qual dispositivo o código é executado de acordo
com a arquitetura. Relação dos principais parâmetros usados no comando
*pgcc*:


  |**Parâmetro**  |  **Descrição** |
  | --------------- | ----------------------------------------------------------- |
  | -fast           | faz a otimização do código                                  |
  | -acc            | habilita o uso de diretivas OpenACC  |
  | -Minfo=accel    | informações sobre quais partes do código foram aceleradas |
  | -Minfo=opt      | informações sobre todas as otimizações de código |
  | -Minfo=all      | informações sobre todas as saídas de código |
  | -ta=host        | compila o código em modo serial |
  | -ta=multicore   | compila o código usando *threads* em CPU |
  | -ta=nvidia      | compila o código usando NVIDIA |

Alguns exemplos de uso do compilador PGI e seus parâmetros básicos.

``` {style="verbatim"}
$ pgcc -acc -ta=nvidia -Minfo=accel main.c
$ pgc++ -acc -ta=nvidia -Minfo=accel main.cpp
```

#### Compilador GCC 

Como dito, uma outra alternativa é o compilador GNU GCC que, sendo um
compilador *opensource*, não requer licença para uso. Para habilitar a
compilação para o OpenACC no GNU GCC, basta especificar o parâmetro
*-fopenacc*.

``` {style="verbatim"}
$ gcc -fopenacc main.c -o teste
```

Se o programa necessitar de funções da biblioteca matemática dentro do
código enviado para o acelerador, use o comando:

``` {style="verbatim"}
$ gcc -fopenacc -foffload=-lm main.c -o teste
```

Para sua execução basta apenas invocar o nome do programa executável na
linha de comando. O mesmo será executado pelo processador hospedeiro e
as partes que devem ser executadas pelo acelerador serão automaticamente
enviadas, sem necessidade de nenhuma configuração adicional.

### Variáveis de ambiente 

#### Compilador PGI 

Para o compilador PGI as seguintes variáveis de ambiente podem ser
utilizadas:

-   **PGI_ACC_TIME**

    Se a variável de ambiente PGI_ACC_TIME for definida como 1, será
    impresso um relatório com o tempo gasto para a movimentação de dados
    entre o hospedeiro e o acelerador, além do tempo de computação no
    acelerador. No Linux, pode ser necessário definir a variável de
    ambiente LD_LIBRARY_PATH para incluir o diretório onde está a
    biblioteca compartilhada *libnvrtc.so*, para implementar o recurso
    de criação de perfil. Esse diretório varia de acordo com a versão do
    compilador, sendo necessário consultar a documentação para
    determinar o caminho correto.

-   **PGI_ACC_DEBUG**

    Se for definido como 1, serve para instruir o ambiente de execução
    do PGI para gerar informações sobre a alocação de memória do
    dispositivo, movimentação de dados, inicialização do *kernel* e
    muito mais. PGI_ACC_DEBUG é utilizado principalmente para uso na
    depuração da sua execução, mas pode ser útil para entender como o
    programa interage com o acelerador.

-   **ACC_DEVICE_TYPE**

    A variável ACC_DEVICE_TYPE tem o valor padrão *acc_device_nvidia* no
    compilador da PGI, exatamente como a opção do compilador -acc tem
    como destino *-ta=tesla* como arquitetura padrão. Os tipos de
    dispositivo *acc_device_default* e *acc_device_not_host* se
    comportam da mesma forma que *acc_device_nvidia*. O tipo de
    dispositivo pode ser alterado usando a variável de ambiente ou por
    uma chamada para a função **acc_set_device_type()** (Veja a Seção
    [\[sec:acc_funcoes\]](#sec:acc_funcoes){reference-type="ref"
    reference="sec:acc_funcoes"}).

-   **ACC_DEVICE_NUM**

    No compilador da PGI, a variável ACC_DEVICE_NUM tem o valor padrão
    '0' para o dispositivo de tipo *acc_device_nvidia*, o que é
    consistente com o sistema de numeração de dispositivos CUDA. Para
    obter mais informações, consulte a saída do comando *pgaccelinfo*. O
    número do dispositivo pode ser alterado usando esta variável de
    ambiente ou por uma chamada para a função **acc_set_device_num()**.

-   **PGI_ACC_DEVICE_TYPE**

    Define o tipo de dispositivo padrão a ser usado. A variável de
    ambiente PGI_ACC_DEVICE_TYPE se sobrepõe à variável ACC_DEVICE_TYPE.
    Ela controla qual dispositivo acelerador deve ser usado ao se
    executar regiões aceleradoras, se o programa foi compilado para usar
    mais de um tipo diferente de dispositivo. O valor desta variável de
    ambiente é definido pela implementação e, atualmente, pode ser um
    dos seguintes valores: NVIDIA, TESLA ou HOST, em caixa alta mesmo.

    O tipo de dispositivo pode ser alterado usando essa variável de
    ambiente ou ainda por uma chamada para a função
    **acc_set_device_type (devicetype)**, onde *devicetype* no
    compilador PGI pode ter um dos seguintes valores:

     |                                |                             |
     | ------------------------------ | --------------------------- |
     | acc_device_none = 0            | acc_device_default = 1 |
     | acc_device_host = 2            | acc_device_not_host = 3 |
     | acc_device_nvidia = 4          | acc_device_pgi_opencl = 7 |
     | acc_device_nvidia_opencl = 8   | acc_device_opencl = 9 |
     | acc_device_current = 10        |                             |

-   **PGI_ACC_DEVICE_NUM**

    Essa variável controla o número do dispositivo padrão a ser usado
    para executar as regiões enviadas para o acelerador. O valor desta
    variável de ambiente deve ser um número inteiro não negativo entre
    zero e o número de dispositivos conectados ao hospedeiro. A variável
    de ambiente **PGI_ACC_DEVICE_NUM** se sobrepõe à variável
    **ACC_DEVICE_NUM**.

-   **PGI_ACC_CUDA_GANGLIMIT**

    Define o número máximo de gangues (blocos de *thread* CUDA) que
    serão iniciados para um *kernel*.

#### Compilador GCC 

Para o compilador GCC são consideradas as seguintes variáveis de
ambiente:

-   **ACC_DEVICE_TYPE**

    Os valores possíveis de serem definidos são os seguintes: **nvidia,
    gcn e host**. O primeiro indica o uso de dispositivos aceleradores
    Nvidia PTX (nvptx), o segundo é utilizado para os aceleradores da
    AMD com arquitetura de codinome GCN (*Graphics Core Next* - GPUs
    Fiji e Vega), e o terceiro significa que a execução será feita por
    uma única *thread* no hospedeiro.

    O tipo de dispositivo pode ser alterado usando esta variável de
    ambiente ou por uma chamada para a função **acc_set_device_type
    (devicetype)**, onde *devicetype* no compilador GCC pode ter um dos
    seguintes valores:
    
     |                           |                         |
     | ------------------------- | ----------------------- |
     | acc_device_current = -1   | acc_device_none = 0     |
     | acc_device_default = 1    | acc_device_host = 2 |
     | acc_device_not_host = 4   | acc_device_nvidia = 5 |
     | acc_device_radeon = 8     |                        |

-   **ACC_DEVICE_NUM**

    Esse valor pode ser definido para um número inteiro não-negativo,
    designando um dispositivo acelerador específico para ser utilizado
    (se houve mais de um instalado). Será emitido um erro de execução de
    o número especificado não corresponder a nenhum acelerador
    instalado.

-   **GOMP_OPENACC_DIM**

    Sobrescreve as dimensões padrão para computação das regiões enviadas
    para o acelerador. Os valores padrão são definidos em tempo de
    execução depois de examinar os recursos de *hardware* disponíveis. O
    valor ótimo para uma determinada aplicação pode não ser o padrão.
    Exemplos:

    ``` {style="verbatim"}
    $ export GOMP_OPENACC_DIM="gang:worker:vector" 
    $ export GOMP_OPENACC_DIM="5120:1:32"
    ```

-   **GOMP_DEBUG**

    A definição de um valor de **GOMP_DEBUG=1** pode ser adicionado às
    variáveis de ambiente para habilitar a depuração durante a execução
    do programa. Serão exibidos a movimentação de dados, o lançamento de
    *kernels* para execução e, para os dispositivos NPTX ativos, exibe o
    código enviado para o acelerador também.

[^1]: https://developer.nvidia.com/hpc-sdk
