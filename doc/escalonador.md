# Escalonamento no nanvix

## Fila de prioridades

O algoritmo fila de prioridades foi implementado usando alguns atributos já existentes no estrutura de processos do nanvix, como o priority e o nice.
Para os processos criados são atribuídos o ao campo priority a prioridade base do nanvix + o valor de nice do processo.
Para escolher qual processo será preemptado o algoritmo percorre a tabela de processos existentes e escolhe aquele que tiver a maior prioridade.
