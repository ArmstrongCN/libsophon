#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <bmlib_runtime.h>

#ifdef __linux__
#define MAX_CHIP_NUM 256

void *bmcpu_pre_start(void *arg) {
    bm_handle_t handle;
    bm_status_t ret;
    int dev_id = *(int *)arg;
    const char* dev = "/dev/bm-sophon";
    const char*       kernel_path = "/opt/sophon/libsophon-current/data";
    char fip_path[100];
    char ramdisk_path[100];
    char dev_path[30];
    bm_cpu_status_t status;

    sprintf(dev_path, "%s%d", dev, dev_id);
    sprintf(fip_path, "%s%s", kernel_path, "/fip.bin");
    sprintf(ramdisk_path, "%s%s", kernel_path, "/ramboot_rootfs.itb");

    while (access(dev_path, F_OK) < 0) {
        sleep(1);
    }
    printf("%s exist! run continue\n", dev_path);

    printf("fip path: %s; \nkernel path: %s\n", fip_path, ramdisk_path);
    ret = bm_dev_request(&handle, dev_id);
    if ((ret != BM_SUCCESS) || (handle == NULL)) {
        printf("bm_dev_request error, ret = %d\r\n", ret);
        return (void *)BM_ERR_FAILURE;
    }

    status = bmcpu_get_cpu_status(handle);
    if (status != BMCPU_IDLE) {
        printf("chip %d bmcpu status is not idle!\n", dev_id);
        ret = bmcpu_reset_cpu(handle);
        if (ret != BM_SUCCESS) {
            printf("reset cpu failed!\r\n");
            return (void *)BM_ERR_FAILURE;
        }
    }

    ret = bmcpu_start_cpu(handle, fip_path, ramdisk_path);
    if ((ret != BM_SUCCESS) && (ret != BM_NOT_SUPPORTED)) {
        printf("start cpu %d failed!\r\n", dev_id);
        bm_dev_free(handle);
        return (void *)BM_ERR_FAILURE;
    }

    bm_dev_free(handle);

    return (void *)BM_SUCCESS;
}

int main(void) {
    int    dev_num;
    int    i;
    int    arg[MAX_CHIP_NUM];
    pthread_t threads[MAX_CHIP_NUM];
    int    ret;

    if (BM_SUCCESS != bm_dev_getcount(&dev_num)) {
        printf("no sophon device found! when sophon device plugin in, sophon-rpc will run!\n");
        return 0;
    }

    if (dev_num > MAX_CHIP_NUM) {
        printf("too many device number %d!\n", dev_num);
        return -1;
    }

    for (i = 0; i < dev_num; i++) {
        arg[i] = i;
        ret = pthread_create(&threads[i], NULL, bmcpu_pre_start, (void *)&arg[i]);
        if (ret != 0) {
            printf("create thread %d error!\n", i);
            return -1;
        }
    }

    for (i = 0; i < dev_num; i++) {
        ret = pthread_join(threads[i], NULL);
        if (ret < 0) {
            printf("join thread %d error!\n", i);
            return -1;
        }
    }

    return 0;
}
#endif
