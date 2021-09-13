// Monitor que implementa a logica do padrao leitores/escritores
class Monitor 
{
  private int leit;
  private int escr;
  
  // Construtor
  Monitor() 
  { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id, String type) 
  {
    try 
    { 

      while (this.escr > 0) 
      {
      // if (this.escr > 0) {
         System.out.println (type + " " + id + " is BLOCKED.");
         
         wait();  //bloqueia pela condicao logica da aplicacao 
      }

      this.leit++;  //registra que ha mais um leitor lendo

      System.out.println (type + " " + id + " is READING.");

    } 
    catch (InterruptedException e) {}
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id, String type) 
  {
     this.leit--; //registra que um leitor saiu

     if (this.leit == 0) 
     {
        this.notify(); //libera escritor (caso exista escritor bloqueado)
     }
           
     System.out.println (type + " " + id + " LEFT.");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id, String type)
  {
    try 
    { 
      while ((this.leit > 0) || (this.escr > 0)) 
      {
      // if ((this.leit > 0) || (this.escr > 0)) {
         System.out.println (type + " " + id + " is BLOCKED.");

         wait();  //bloqueia pela condicao logica da aplicacao 
      }

      this.escr++; //registra que ha um escritor escrevendo

      System.out.println (type + " " + id + " is WRITING.");

    } 
    catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id, String type) 
  {
     this.escr--; //registra que o escritor saiu

     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)

     System.out.println (type + " " + id + " LEFT.");
  }
}
