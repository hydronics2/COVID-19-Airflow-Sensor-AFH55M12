# Airflow Sensor - Automotive Sensor AFH55M12

## Project Description ([from Helpful Engineering](https://www.helpfulengineering.org/))
The intent here is to create a monitoring device, based on a mass airflow meter, that can be used when splitting a ventilator into two or more patients. This will allow staff to monitor individual patients while being controlled by one device in extreme situations where the number of ventilators are not enough to handle the number of patients. The readout should be visible locally on the device and there may need to be parameters input by staff to create a safe operating range and to possibly create alarms when the system is measuring an out of range parameter.


### [Project Requirements](https://docs.google.com/document/d/17Ps910A2vRwnM4EM6F-71GNG1XNa0PaeImd53F7428c/edit?usp=sharing)

This is a quick study of using an inexpensive off-the-shelf automotive airflow type sensor.

Reading from an automotive mass airflow sensor using a microController 12bit ADC, 20ms interval


### --- Updates April 8th -- Assembled PCB, airflow sensor, on 3d print

The model by @john Grano is [here](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/3d-prints/MAF_Chamber_Body_V3.stl). Although this appears to prove the technology is working, we're currently pivoting to a manifold type layout...
- to centralize all user interface displays next to the mechanical ventilator
- simplifies to one PCB
- one power supply
- serves multiple flow sensors
[![Foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/airflow_video2.PNG)](https://youtu.be/e5O91pYZDbE)


### --- Updates April 6th -- 3D print by @john Grano on Slack

![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/3d_print.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/3d_print2.PNG)
![foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/3d_print3.PNG)


###  --- Updates April 4th -- Arduino Sketch show airflow
The arduino sketch shows estimated ariflow volume in mL.

[![Foo](https://github.com/hydronics2/COVID-19-Airflow-Sensor-AFH55M12/blob/master/pics/airflow_video.PNG)](https://youtu.be/0604_5CF9F4)



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
The sensor shows up as multiple part values. It fits a 1998 Nissan/Datsun Truck Frontier 2WD 2.4L FI 4cyl among other years and other vehicles.
- [China](https://www.aliexpress.com/item/32989444018.html?spm=a2g0o.cart.0.0.74dd3c00g1XL5u&mp=1)
- [US Supplier](https://www.amazon.com/gp/product/B074FZMGWG/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
- [local supplier oreilly](https://www.oreillyauto.com/search/1998/nissan/frontier?q=MF21219)
- [local supplier auto-zone](https://www.autozone.com/engine-management/mass-air-flow-sensor/duralast-new-mass-air-flow-sensor-dl-3025/337334_0_4995?&searchText=DL3025)

#### Pinout
- Pin A - Empty
- Pin B - 7-12V DC, 90-115mA
- Pin C - Ground
- Pin D Signal

#### Parts List
- oled display is a generic [0.96" 128X64 I2C OLED](https://www.ebay.com/sch/i.html?_from=R40&_trksid=m570.l1313&_nkw=0.96%22+128X64+I2C+OLED&_sacat=0)
- BME280 pressure sensor [828-1063-1-ND](https://www.digikey.com/product-detail/en/bosch-sensortec/BME280/828-1063-1-ND/6136314)
