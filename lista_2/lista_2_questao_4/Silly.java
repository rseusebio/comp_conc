public class Silly
{
    private double powerDegree;

    Silly()
    {
        this.powerDegree = 8;
    } 
    
    void go()
    {
        double silly = Math.pow(10, this.powerDegree);

        double silly2 = -1 * silly;

        while(silly2 <= silly) silly2 += 1;
    }
}
