package app;

import lombok.AllArgsConstructor;

@AllArgsConstructor
public class Producer extends Thread {
    private Warehouse warehouse;
    private byte ID;
    private final Object lock;

    @Override
    public void run() {
        while (true) {
            try {
                sleep((long)(Math.random() * 5000));
                String product = "";
                int result = (int)(Math.random() * 3);
                switch (result) {
                    case 0 -> product = "flour";
                    case 1 -> product = "olive oil";
                    default -> product = "tomatoes";
                }
                int amount = 1 + (int)(Math.random()*10);
                int availableSpace= warehouse.receiveProducts(product, amount);
                if (amount==availableSpace) {
                    //System.out.println(amount+" pieces of "+product+" has been produced and delivered to warehouse.");
                }
                else if(availableSpace!=0){
                    //System.out.println("Only" + availableSpace + " out of "+amount+" produced pieces of "+product+
                    //        " has been delivered to warehouse due to lack of space.");
                }
                else{
//                    System.out.println("Everything ("+amount+ " pieces of " + product
//                            +") got wasted due to lack of space in warehouse.");
                    System.out.println("Producer" + ID +" could not deliver what he produced. There is no space in the " +
                            "warehouse. He is waiting...");
                    synchronized (lock) {
                        while ((availableSpace=warehouse.receiveProducts(product, amount))==0) {
                            lock.wait();
                        }
                        System.out.println("Producer"+ID+" after waiting delivered "+availableSpace+" pieces of "+product);
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}