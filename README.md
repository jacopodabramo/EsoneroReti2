# Documentazione
Repository riguardante la seconda parte dell'esonero (protocollo UDP) dell'esame di reti di calcolatori.<br>
Il software realizzato è un'applicazione Client/Server, in cui il server è una calcolatrice capace di effettuare operazioni di somma, sottrazione, moltiplicazione e divisione tra numeri interi; mentre il Client si occupa di inviare la stringa contenente l'operazione e gli operandi, nella giusta sintassi, al Server.<br>
 
 ## Spiegazione di alcune scelte
Visto che la calcolatrice effettua operazione fra numeri interi, si è deciso di gestire il risultati delle operazioni attraverso numeri reali _(float)_. <br>
Le operazioni sono definite in questo modo:
- +: Intero x Intero -> Reale
- x: Intero x Intero -> Reale
- /: Intero x Intero -> Reale
- -: Intero x Intero -> Reale <br>

La _divisione per zero_ visto che è un'operazione che non può essere svolta, si è deciso che il server restuisca il massimo numero reale che può essere rappresentato in C, in modo tale che il Client una volta che riceve questo numero può stampare un messaggio di errore. <br><br>


Per evitare che l'utente inserisse dei valori troppo elevati che causerebbero un _overflow_ si è deciso che l'utente può inserire un range di valori che va da -10.000 a + 10.000 .<br><br>

Per questione grafiche  si è deciso di stampare le prime due cifre decimali dopo  la virgola nel _Client_.<br><br>

_All' avvio_, al Client può essere passato un parametro da _Comand Line_ nel seguente formato <code>srv.di.uniba.it:56700</code>.<br>
La prima parte della stringa fino ai <code>:</code> indica il nome del Server, la seconda parte della stringa indica  _il numero di porta_; nei casi in cui la stringa si dovesse trovare in un formato errato (_Es_ <code>srv.di.uniba.it:56ab0</code> ), oppure non dovesse essere passato questo parametro verrano utilizzati _indirizzo ip_ e _numero di porta_ di default corrispondenti a:
- _127.0.0.1 per l'indirizzo ip_ 
- _56700 per il numero di porta_ <br><br>


Per il controllo della stringa inserita dall'utente, si è deciso, come da requisiti, che la stringa verrà ritenuta corretta esclusivamente nel seguente formato:<br> 
<p align = "center"><code>[Operazione][Spazio][Primo operando][Spazio][Secondo operando]</code> </p> 
<br>

_Esempi di stringhe accettate_:
- <code>+ 10 20</code>
- <code>x -10 20</code>
- <code>- -10 -20</code><br>

_Esempi di stringhe non accettate:_ 
- <code>++ 10 20</code>
- <code>+ 10 20aaa</code>
- <code>- --10 20</code> <br>

## Autori
L'esonero è stato realizzato da:
- Lorenzo Cassano (matricola 718331)
- Jacopo D'Abramo (matricola 716484)
