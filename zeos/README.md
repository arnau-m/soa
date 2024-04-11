
Classe 21/03/2024

TEORIA INICI CLASSE
Per reduir la quantitat de mem que fa servir la taula de pagines en x86 no fem servir un vector com hem vist a teoria, sinó que es definida pel processador en diferents nivells:
    Nivell 1: Directorio (1024 entrades) -> Ens diu si d'entre els troços de la taula de pàgines hi ha realment alguna traducció per evitar guardar a memoria fragments sense informació.
    A cadascuna de les posicións de la taula tenim la direcció a la taula de pàgines i un bit per indicar si existeix alguna traducció vàlida. 
    Assignar un directori o una taula de pàgines fa servir les mateixes funcións. 

TODO
    - Hem de fer un wrapper que sigui task_switch que guardi ebx, esi i edi i cridi a inner_task_switch
    - inner_task_switch es pot fer una part en alt nivell, pero al canviar la pila de sistema hem de fer-ho amb assembly. Un tros estarà al sched.c i l'altre part final estara en un .S.

    schedule(){
        update_sched_data()
        if (needssched()){
            update_process_state()
            sched_next()
        }
    }

    Hem de fer tb les syscalls block i unblock.