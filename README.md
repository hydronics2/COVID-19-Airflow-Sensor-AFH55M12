# Airflow Sensor - Automotive Sensor AFH55M12

## Project Description ([from Helpful Engineering](https://www.helpfulengineering.org/))
The intent here is to create a monitoring device, based on a mass airflow meter, that can be used when splitting a ventilator into two or more patients. This will allow staff to monitor individual patients while being controlled by one device in extreme situations where the number of ventilators are not enough to handle the number of patients. The readout should be visible locally on the device and there may need to be parameters input by staff to create a safe operating range and to possibly create alarms when the system is measuring an out of range parameter.


### [Project Requirements](https://docs.google.com/document/d/17Ps910A2vRwnM4EM6F-71GNG1XNa0PaeImd53F7428c/edit?usp=sharing)

This is a quick study of using an inexpensive off-the-shelf automotive airflow type sensor.

Reading from an automotive mass airflow sensor using a microController 12bit ADC, 20ms interval

###  --- Updates Wednesday March 28th -- Airflow testing

Installed sensor in a 1" tube. Only showing inhales
- used previous estimates for a medium inhale breath as ~1150mL to calibrate sensor

![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/airFlow_graph.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/sensor_diagram.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/bench_setup1.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/bench_setup2.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/sensor1.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/sensor2.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/airflow_sensor_integration.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/3dmodel_top.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/3dmodel_bottom.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/3dmodel_side.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/schematic.PNG)

### Sensor Availability
- [China](https://www.aliexpress.com/item/32989444018.html?spm=a2g0o.cart.0.0.74dd3c00g1XL5u&mp=1)
- [US Supplier](https://www.amazon.com/gp/product/B074FZMGWG/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)

#### Pinout
- Pin A - Empty
- Pin B - 7-12V DC, 90-115mA
- Pin C - Ground
- Pin D Signal
