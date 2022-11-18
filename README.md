## Mosquitto MQTT Subscriber Example (Sample 4)   
### Start MQTT Broker:   

On localhost machine run mqtt broker   
```
cd ../mqtt_subscriber/mqtt
docker-compose up -d
```
### Run

Set in einit/src/init.yaml.in your local ip address and 18883 port (it forwarded via Docker) (for me its 10.5.20.23)...   
```
# The `client` entity can call `server`.
- name: echo.Client
  env:
    VFS_FILESYSTEM_BACKEND: client:@ROOTFS_ENTITY@
    VFS_NETWORK_BACKEND: client:kl.VfsNet
    MQTT_BROKER_ADDRESS: "10.5.20.23"
    MQTT_BROKER_PORT: "18883"
```
You can check it with  
```
ip a
```
If you`re linux user :)   

Run docker image with:
```
docker run -it --rm --volume $(pwd):/workspace --privileged kos:latest bash   
```

Build n run subscriber:   

```
./cross-build.sh   
/opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/qemu-system-aarch64 -m 2048 -machine vexpress-a15,secure=on -cpu cortex-a72 -nographic -monitor none -smp 4 -net nic -net user -serial stdio -kernel build/einit/kos-qemu-image   
```

### Write numbers into broker   

Client subscribed on `my/awesome/topic`..

Run from your local machine ` mosquitto_pub -h localhost -p 18883 -t 'my/awesome/topic' -m '<number>'` (require mosquitto_pub: apt-get install -y mosquitto-clients)   

You should see something as [ScreenShot1](/mqtt_subscriber_echo/screens/1.png "Spam screen")   
