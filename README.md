# Sistema de iluminação com controle remoto


## Arquivos

### Main.c
O código main.c é utilizado para executar as funcionalidades da ESP 32. O código está descrito em detalhes no relatório na seção ESP 32
 
### Node_red.json
O arquivo node_red.json é referente ao código do fluxo utilizado no node red e também gera o bot do Telegram. 

## Instruções Raspberry

1 - Primeiramente é necessário a instalação de dois softwares na raspberry, a qual pode possuir qualquer sistema operacional. O primeiro deles é o node-red cuja instalação e execução depende do Nodejs e npm. Tendo os dois instalados basta executar o comando para a instalação:

```  sudo npm install -g --unsafe-perm node-red  ```

2 - O segundo software é o Broker mosquitto, responsável pela distribuição das mensagens trocadas entre os dispositivos. Para a instalação deste basta executar o comando:

```  snap install mosquitto  ```

Para o funcionamento correto de todo o projeto a Raspberry deve possuir o endereço de IP fixo na rede, o IP utilizado foi 162.168.0.100.

3 - Após essas configurações da raspberry podemos dar inicio a configuração do projeto em si. Para carregar o fluxo do node-red é necessário primeiramente executar o node-red na raspberry, em seguida checar o IP da raspberry e fazer acesso a partir do Browser, usando o IP_RASPBERRY:1880, caso tenham sido usadas as configurações padrão do node-red. 

4 - Após o node-red ser executado é necessária a instalação de dois pacotes adicionais de nós. Para isso basta acessar o menu, no canto superior direito da plataforma, clicar em Manage Pallet, e buscar pelos pacotes: node-red-dashboard e node-red-contrib-chatbot .
	Em seguida para carregamento do fluxo, é necessário clicar em Import, no mesmo menu usado anteriormente e selecionar o arquivo node_red.json. Com isso estarão carregados os flows de controle da interface e também o do chatBot. 

5 - Em seguida basta clicar em Deploy, para tudo estar funcional. Para o uso do chatBot a raspberry necessita de uma conexão com a internet. O acesso acontece através do link: t.me/controle_luzes_bot . 



## Instruções ESP32

Primeiramente, é preciso importar o código no Arduino IDE. Para instalar e configurar o Arduino IDE, basta seguir as orientações da seção ESP32 no relatório. 

### Conexões físicas

As conexões devem ser feitas da seguinte forma:

1 - A ESP, os sensores e o relé devem ser alimentados por uma tensão de 5V nas entradas “Vin” e por 0V nas entradas GND.

2 - O relé e a ESP devem ser conectados através das portas digitais definidas no código da ESP. Essa saídas conectam-se às entradas controladoras do relé. Cada saída digital controla uma (ou mais) lâmpadas. As saídas da ESP dependem do código e o nome das entradas do relé dependem do modelo utilizado.

3 - Os sensores de proximidade devem ser conectados pela porta de saída às entradas digitais definidas no código da ESP.

4 - As controladoras do relé devem ser conectadas à alimentação de 12V de um lado e às resistências dos LEDs (ou às lâmpadas) do outro. 

5 - Com a tensão de 12V é possível alimentar até 5 LEDs em série após à resistência. O último LED da série deve ser conectado à tensão de 0V.

A maioria das conexões físicas depende da estrutura de código utilizada e das tensões de referência aplicadas. Recomenda-se seguir as portas 25, 26, 32 e 33 para o controle digital das saídas conectadas ao relé e a porta 17 (tx) para a leitura do sensor de presença.

### Alterações necessárias no código da ESP

O código da ESP necessita de alteração para realizar a conexão à rede wifi necessária. A figura 15 do relatório especifica as linhas que demandam o nome e a senha da rede.

A mesma lógica serve para o IP do MQTT configurado, bem como o nome de cada um dos tópicos aos quais a ESP deve ser inscrito (vide figura 15 do relatório).

O número das portas digitais para entrada e saída de dados também pode ser alterado, mas seu funcionamento não é garantido. Recomenda-se fortemente a utilização das portas citadas na seção anterior.
