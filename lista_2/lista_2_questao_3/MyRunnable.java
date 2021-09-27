import java.lang.Math;

class MyRunnable implements Runnable
{
    public void run()
    {
        double silly = Math.pow(10, 8);
        double neg_silly = -1 * silly;

        while(neg_silly <= silly)
        {
            neg_silly += 1;
        }
    }
}