import java.util.Scanner;
import java.util.Vector;
import java.util.List;
import java.util.stream.Stream;

/**
 * Thread responsável por fazer a leitura 
 * do arquivo de entrada e inserir no buffer 
 * compartilhado cada linha lida.
 */
class Producer extends Thread 
{
  int id;
  ProdConMonitor monitor;
  int rows;
  Scanner scanner;


  Producer(int id, ProdConMonitor m, Scanner s, int r) 
  {
    this.id = id;
    monitor = m;
    scanner = s;
    rows = r;
  }

  public void run() 
  {
    try
    {

      while (rows > 0)      
      {
        String[] strArray = scanner.nextLine().split(" ");

        List<Integer> list = Stream.of(strArray).map(Integer::valueOf).toList();

        Vector<Integer> item = new Vector<>(list);
        
        monitor.Insert(id, item);

        rows--;
      }
    }
    catch (Exception er)
    {
      System.out.println("Producer error: " + er.getMessage());

      er.printStackTrace();

      System.exit(-1);
    }
  }
}