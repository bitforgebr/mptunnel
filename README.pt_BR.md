# mptunnel
MultiPath Tunnel (Simpified user space MPUDP)

[中文说明点此](README.zh_CN.md)
[README in English](README.md)

## SOBRE

Túnel 
MultiPath Tunnel é uma implementação em user space de UDP multipath. Como MultiPath TCP, você pode
estabilizar várias conexões do servidor local para o remoto.

MPTCP(MultiPath TCP) é uma boa ideia para tornar a conexão de rede mais robusta, mas apenas funciona
em TCP. Foi através da busca por uma implementação de MPUDP que foi escrita essa ferramenta.


## CONCEPÇÃO

```
                        .---- bridge server 1 ----.
                       /                            \
 Server A --- mpclient ------- bridge server 2 ------- mpserver --- Server B
                       \                            /
                        `---- bridge server 3 ----`
```

Existem dois servidores Server A e Server B. A conexão de rede entre Server A e Server B é instável (com
uma razão alta de perda de pacotes). Dessa forma, nós gostaríamos de estabilizar um túnel multipath entre
Server A e Serer B, esperando que a conexão entre Server A e Server B se torne mais estável (diminua a razão
de perda de pacotes).

_mpclient_ é a parte cliente do mptunnel, ele pode rodar no ServerA. Você deve dizer ao mpclient a informação
dos servidores bridge. Uma vez que o mpclient é iniciado, ele abre uma porta local UDP para listen e redireciona
qualquer pacote de/para os servidores bridge.

_mpserver_ é a parte servidora do mptunnel, ele pode rodar no ServerB. Você deve dizer ao mpserver a
informação do Server B. Uma vez que mpserver é iniciado, ele irá redirecionar qualquer pacote de/para
o Server B.

Os servidores bridge são simples, eles apenas redirecionam os pacote do mpclient para mpserver, ou pacotes do
mpserver para mpclient. Você pode usar _nc_ ou _socat_ para entregar um servidor bridge.


## EXEMPLO

Eu quero conectar em meu servidor OpenVPN, mas a conexão é instável e a razão de perda de pacotes é alta. O throughput
TCP sobre o túnel OpenVPN é muito pequeno devido à alta perda de pacotes. Para aumentar o throughput TCP (diminuir a
perda de pacote), eu posso rodar um MPUDP para o servidor OpenVPN e estabilizar uma conexão OpenVPN nele.

OpenVPN está em listen na porta 1194 UDP, eu executo mpserver no servidor OpenVPN da seguinte forma:

```
mpserver 2000 127.0.0.1 1194
```

Localmente, eu executo o mpclient:

```
mpclient 4000 mpclient.conf
```

Abaixo está o conteúdo do arquivo mpclient.conf 

```
# mptunnel

localhost 4001
localhost 4002
localhost 4003
```

Em cada servidor bridge, use _socat_ para redirecionar os pacotes:

```
socat udp-listen:4001 udp4:1.1.1.1:2000
socat udp-listen:4002 udp4:1.1.1.1:2000
socat udp-listen:4003 udp4:1.1.1.1:2000
```

Os servidores bridge irão ficar em listen nas portas 4001, 4002 e 4003 e redirecionar qualquer pacote recebido para 1.1.1.1:2000, 
e vice-versa.


Agora eu faço o cliente OpenVPN conectar em localhost:4000 que o mpclient está em listen, e a OpenVPN irá estabiizar
uma conexão OpenVPN sobre o MultiPath UDP tunnel.


## BUGS

* mptunnel adiciona alguma informação de controle dentro dos pacotes, incluindo informação síncrona. mpserver e mpclient devem ser iniciados ao mesmo tempo. Se o mpclient ou o mpserver terminar, você terá que reiniciar ambos para restabelecer o túnel.

* Atualmente você pode especificar apenas um único host alvo. Alguém sabe se existe uma biblioteca C de proxy SOCKS5? Penso que ao tornar o mpclient como um servidor proxy SOCKS irá torná-lo mais fácil de usar.

* mptunnel não encripta os pacotes por padrão, apesar de ter essa opção, pois isso irá diminuir o throughput. Em alguns testes o throughput atual é 3Mbps enquanto usando três túneis com criptografia, e após desabilitar a criptografia o throughput sobe para 300Mbps. Se você ainda quiser que o mptunnel encripte os pacotes, defina a variável de ambiente MPTUNNEL_ENCRYPT=1.

## DEPENDÊNCIAS

Para compilar o mptunnel, essas bibliotecas são requeridas:

* libev

## VEJA TAMBÉM

[mlvpn](https://github.com/zehome/MLVPN/), uma solução similar para multipath UDP.
