
Codigo com C, monitor de hardware: Linux

=> Este é um codigo que exibe:

 . temperatura(usando libsensors)
 . uso de memória RAM
 . Carga média do sistema operacional(load average)

 Resumo Geral:

 recurso			origem/função usadas		descrição
	
Temperatura		libsensors		Buscar sensosores de temperatura.	
RAM			Lê /proc/meminfo	Calcula o uso da RAM baseado em MemTotal, MemAvailable.
Carga			getloadagv()		Mostra a carga média do OS.
Atualização		sleep(2) + clear	Atualiza as informações a 2seg.

=> Requisitos para rodar:
 .Sistema linux; biblioteca libsensors-dev
 .permissão de leitura para /proc/meminfo.


=> Analise do Codigo: monitor.c
 Esses #include's importam as bibliotecas necessarias.
  .stdio.h, stdlib.h, string.h: função padrão de entrada, saída, alocação de memória e manipulação de strings.
  .sensors/sensors.h: biblioteca libsensors para obter dados de sensores do sistema(como tempuratura).
  .unistd.h: fornece sleep() e outras funções POSIX.


=> Função: view_temp()
  .Detecta os chips com sensores disponíveis no sistema(sensors_get_detect_chips).
  .Para cada chip, percorre suas "features" (caracteristicas do sensor).
  .Verifica se a features é do tipo SENSORS_FEATURES_TEMP (temperatura).
  .Pega a subfeature de entrada de temperatura(TEM_INPUT).
  .Lê o valor da temperatura com sensors_get_value() e exibe no console.

=> Função: view_memory()
  .Abre o arquivo /proc/meminfo na raiz.
  .Lê linha por linha e extra os valores em MemTotal(memória total), MemAvailable(memória disponível).
  .Calcula e imprime na tela RAM usada no momento.

=> Função: view_load()
  .Usa getloadavg() para pegar a média de carga do sistema nos últimos:
    1 minutos, 5 minutos e 15 minutos.

  .Exibe esses valores
    load average mostra quantos processos estão por CPU. Valores próximos ou maiores que o número de núcleos indicam uso elevado.

=> Função: main()
  .inicializa o script. Em um laço infinito.
  .chama as funções acima.
  .Quando sair do loop(nunca ocorre aqui) chama sensors_cleanup para liberar recursos.


=> Possíveis erros: na compilação
 /usr/bin/ld: monitor.c: (.text+x0.74)
 /usr/bin/ld: monitor.c: (.text+x0.a4)
 /usr/bin/ld: monitor.c: (.text+x0.c4)
 /usr/bin/ld: /tmp/ccvcjOWU.o: "main"
 monitor.c:(.text+x0x2c7)
 collect2:error:ld returned 1 exit status

 Este é um erro, que talvez a maquina não possua o libsensors-dev instalado.
  .sudo apt-get install libsensors-dev

 Verificar se foi instalado corretamente o libsensors
  .ldconfig -p | grep libsensors
 saida será algo assim:
   libsensors.so.5(libc6, x86~64) => /lib/x86_64-linux-gnu/libsensors.so.5
   libsensors.so(libc6, x86~64) => /lib/x86_64-linux-gnu/libsensors.so


=> Faça a compilação:
  .sudo gcc monitor.c -o monitor -lsensors
  A flag -lsensors no final é essencial para dizer ao compilador que você quer linkar com a biblioteca libsensors.


