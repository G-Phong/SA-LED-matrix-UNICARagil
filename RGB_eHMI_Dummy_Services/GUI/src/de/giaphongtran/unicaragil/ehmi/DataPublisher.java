package de.giaphongtran.unicaragil.ehmi;

import java.util.ArrayList;

public class DataPublisher<T> {

	private T data;
	private ArrayList<DataSubscriber> subscribers;
	
	public DataPublisher() {
		subscribers = new ArrayList<DataSubscriber>();
	}
	
	public DataPublisher(T defaultValue) {
		this();
		data = defaultValue;
	}
	
	public void subscribe(DataSubscriber sub) {
		subscribers.add(sub);
		sub.notifySub();
	}
	
	public void unsubscribe(DataSubscriber sub) {
		subscribers.remove(sub);
	}
	
	public void setData(T data) {
		this.data = data;
		notifySubs();
	}
	
	public void setDataSmartNotify(T data) {
		T oldData = this.data;
		this.data = data;
		
		if (!oldData.equals(data)) {
			notifySubs();
		}
	}
	
	public T getData() {
		return data;
	}
	
	public void notifySubs() {
		for (DataSubscriber sub : subscribers) {
			sub.notifySub();
		}
	}
}
