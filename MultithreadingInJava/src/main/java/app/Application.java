package app;

import java.util.ArrayList;

public class Application{
    private static final ArrayList<Producer> producers = new ArrayList<>();
    private static final ArrayList<Client> clients = new ArrayList<>();

    public static void main(String[] args) {
        Warehouse warehouse = new Warehouse();
        warehouse.start();

        for(byte i=0; i<3; i++) {
            producers.add(new Producer(warehouse, i));
            producers.get(i).start();
            clients.add(new Client(warehouse, i));
            clients.get(i).start();
        }
    }
}