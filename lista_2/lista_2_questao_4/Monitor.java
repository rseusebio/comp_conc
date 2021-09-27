import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

/**
 * todas as vezes que um escritor tentar escrever e existir leitores lendo, a entrada de novos
 * leitores fique impedida ate que todos os escritores em espera sejam atendidos.
 */

public class Monitor 
{
    private int leit;
    private int escr;
    private HashSet<Integer> lista_escr; // Lista de espera de escritores
    private boolean lista_executando;

    Monitor() 
    {
        this.leit = 0; // leitores lendo (0 ou mais)
        this.escr = 0; // escritor escrevendo (0 ou 1)
        this.lista_escr = new HashSet<>();
        this.lista_executando = false;
    }

    // Entrada para leitores
    public synchronized void EntraLeitor(int id) 
    {
        try 
        {
            while (this.lista_escr.size() > 0)  // espera a lista
            {
                System.out.println("L" + id + " bloqueado [pela lista]");
                
                wait();
            }

            while (this.escr > 0) 
            {
                System.out.println("L" + id + " bloqueado");

                wait(); // bloqueia pela condicao logica da aplicacao
            }

            this.leit++; // registra que ha mais um leitor lendo

            System.out.println("L" + id + " lendo");
        } 
        catch (InterruptedException e) {}
    }

    // Saida para leitores
    public synchronized void SaiLeitor(int id) 
    {
        this.leit--; // registra que um leitor saiu

        if (this.leit == 0)
        {       
            this.notifyAll(); // libera escritor ou leitor (caso exista escritor bloqueado)
        }   

        System.out.println("L" + id + " liberou");
    }

    // Entrada para escritores
    public synchronized void EntraEscritor(int id) 
    {
        try 
        {
            while (this.leit > 0 || // impede que escreve quando há leitores lendo
                   this.escr > 0 || // impede que escreve quando há escritores escrevendo (fora da lista)
                  (this.lista_executando && !this.lista_escr.contains(id))) // impede que escreve quando a lista está executando e o escrito em questão não está nela
            {
                if (this.escr <= 0 && !this.lista_executando) // algum leitor esta executando
                {
                    System.out.println("E" + id + " é bloqueado [inserido na lista]");

                    this.lista_escr.add(id);    
                }
                else if(this.lista_executando)
                {
                    System.out.println("E" + id + " é bloqueado [pela lista]");
                }   
                else 
                {
                    System.out.println("E" + id + " é bloqueado [por escritor]");
                }

                wait();
            }


            this.escr++; // registra que ha um escritor escrevendo

            if (this.lista_escr.contains(id))
            {
                this.lista_executando = true; // diz que a lista de espera está executando
            }

            System.out.println("E" + id + " escreve");
        } 
        catch (InterruptedException e) {}
    }

    // Saida para escritores
    public synchronized void SaiEscritor(int id) 
    {
        this.escr--; // registra que o escritor saiu

        if (this.lista_escr.contains(id))
        {
            this.lista_escr.remove(id);

            if (this.lista_escr.size()  == 0)
            {
                this.lista_executando = false; // diz que a lista terminou de executar
            }
        }

        notifyAll(); // libera leitores e escritores (caso existam leitores ou escritores bloqueados)

        System.out.println("E" + id + " liberou");
    }
}
