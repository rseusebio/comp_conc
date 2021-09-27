public class Escritor extends Thread 
{
    int id; //identificador da thread
    int delay; //atraso bobo...
    Monitor monitor; //objeto monitor para coordenar a lógica de execução das threads
  
    // Construtor
    Escritor (int id, int delayTime, Monitor m) 
    {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    // Método executado pela thread
    public void run () 
    {
      try
      {
        while (true) 
        {
          this.monitor.EntraEscritor(this.id); 

          new Silly().go();

          this.monitor.SaiEscritor(this.id); 

          sleep(this.delay); //atraso bobo...
        }
      } 
      catch (InterruptedException e) { return; }
    }
  }
