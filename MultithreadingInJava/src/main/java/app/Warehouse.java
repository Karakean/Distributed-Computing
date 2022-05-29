package app;

import lombok.AllArgsConstructor;

import java.util.HashMap;
import java.util.Map;


public class Warehouse extends Thread {
    private int MAX_CAPACITY = 100;
    private Map<String, Integer> warehouse = new HashMap<>();
    private int amount_in_store = 0;
    private final Object lock;

    Warehouse(Object lock){
        this.lock = lock;
    }

    @Override
    public void run() {
        System.out.println("Warehouse is opened.");
    }

    synchronized int receiveProducts(String product, int received_amount) {
        if (this.amount_in_store + received_amount <= this.MAX_CAPACITY) {
            this.amount_in_store += received_amount;
            int new_amount = received_amount;
            if (this.warehouse.containsKey(product)) {
                new_amount += this.warehouse.get(product);
                this.warehouse.remove(product);
            }
            this.warehouse.put(product, new_amount);
            //printWarehouse();
            //System.out.println(received_amount + " pieces of " + product + " has been added to warehouse.");
            synchronized (lock){
                lock.notifyAll();
            }
            return received_amount;
        } else if (this.amount_in_store != this.MAX_CAPACITY) {
            int amount = this.MAX_CAPACITY - this.amount_in_store;
            this.amount_in_store = this.MAX_CAPACITY;
            int new_amount = amount;
            if (this.warehouse.containsKey(product)) {
                new_amount += this.warehouse.get(product);
                this.warehouse.remove(product);
            }
            this.warehouse.put(product, new_amount);
            //System.out.println("Only " + amount + " out of " + received_amount + " pieces of " + product +
            //        " has been added to warehouse due to lack of space.");
            //printWarehouse();
            synchronized (lock){
                lock.notifyAll();
            }
            return amount;
        } else {
            //System.out.println("Warehouse is full.");
            return 0;
        }
    }

    private void printWarehouse() {
        System.out.println("There is " + this.amount_in_store + " products in the warehouse: ");
        String product = "";
        for (Map.Entry<String, Integer> entry : this.warehouse.entrySet()) {
            System.out.println(entry.getKey()+": "+entry.getValue());
        }
    }

    synchronized int sellProducts(String product, int amount) {
        if (this.warehouse.containsKey(product)) {
            if (this.warehouse.get(product) >= amount) {
                int old_amount = this.warehouse.get(product);
                this.warehouse.remove(product);
                if(old_amount > amount) {
                    this.warehouse.put(product, old_amount - amount);
                }
                this.amount_in_store -= amount;
                //System.out.println(amount + " pieces of " + product + " has been sold.");
                //System.out.println("There is " + this.amount_in_store + " products in the warehouse.");
                synchronized (lock){
                    lock.notifyAll();
                }
                return amount;
            } else {
                int old_amount = this.warehouse.get(product);
                this.warehouse.remove(product);
                //System.out.println(old_amount + " pieces of " + product +" has been sold. There is no " + product + " left.");
                //System.out.println("There is " + this.amount_in_store + " products in the warehouse.");
                this.amount_in_store -= old_amount;
                synchronized (lock){
                    lock.notifyAll();
                }
                return old_amount;
            }
        } else {
            //System.out.println("There is no " + product + " in the warehouse.");
            return 0;
        }
    }
}