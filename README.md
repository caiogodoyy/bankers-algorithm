# Banker's Algorithm
Tem esse nome porque o algoritmo poderia ser usado em um sistema bancário para garantir que o banco nunca alocasse seu caixa disponível de forma que não pudesse satisfazer as necessidades de todos os seus clientes.

Quando uma nova thread entra no sistema, ela deve declarar o número máximo de instâncias de cada tipo de recurso que possa precisar. Este número pode não exceder o número total de recursos no sistema. Quando um usuário solicita um conjunto de recursos, o sistema deve determinar se a alocação desses recursos deixarão o sistema em um estado seguro. Se for, os recursos são alocados; caso contrário, o thread deve esperar até que algum outro thread libere o suficiente Recursos.

- CESAR School
- Software Insfrastructure (Operating Systems)
- Professor Erico Teixeira
- Reference: Operating System Concepts 10th Edition, by Abraham Silberschatz et al (Chapter 8)

## Comandos
+ Clonar o repositório
```bash
git clone https://github.com/caiogodoyy/bankers-algorithm
cd bankers-algorithm/
```
+ Compilar:
```bash
make
```
+ Executar:
```bash
make run
```
+ Limpar após uso:
```bash
make clean
```

<div align="center">
<img src="https://media.giphy.com/media/1U4S8219ByoGk/giphy.gif" width="200px"/>
</div>
