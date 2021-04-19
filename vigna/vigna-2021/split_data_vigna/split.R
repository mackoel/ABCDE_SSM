#отделение тестовой выборки(1666 ВСЕГО, 166 ТЕСТОВАЯ, 1500 ТРЕНИРОВОЧНАЯ И ВАЛИДАЦИОННАЯ)

library(h5)

dfile <- paste0('vigna-2021-v4-vqtl-utf.h5')
file <- h5file(dfile)

year <- file["year"][]
doy = file["doy"][]
month = file["month"][]
geo_id <-file["geo_id"][]


gr_names <-  file["gr_names"][]
gr_covar <- file["gr_covar"][]
response_EM <- file["response_EM"][]
response_R1 <- file["response_R1"][]
response_R3 <- file["response_R3"][]
response_R5 <- file["response_R5"][]
response_R7 <- file["response_R7"][]
response_R8 <- file["response_R8"][]

species <-file["species"][]
h5close(file)

index <- c(1:1666)
index <- sample(index, replace = FALSE)
index_test <- index[1:166]
index_tr_val <- index[167:1666]

hist(response_R1[index_test], ylim = c(0, 300), xlim=c(10,80), col="red")
hist(response_R1[index_tr_val], add=T, col=rgb(0, 1, 0, 0.5) )

dfile <- paste0('vigna-2021-v4-vqtl-utf-test.h5')
file <- h5file(dfile)

file["year"] <- as.matrix(as.integer(year[index_test]))
file["doy"]   <- as.matrix(as.integer(doy[index_test]))
file["month"] <- as.matrix( as.integer(month[index_test]))
file["geo_id"] <-as.matrix( as.integer(geo_id[index_test]))

#gr_names <- c("snp1", "snp2","snp3","snp4","snp5","snp6","snp7","snp8","snp9","snp10")
file["gr_names"]<- gr_names
file["gr_covar"] <- as.matrix(gr_covar[index_test,])
file["response_EM"]<- as.matrix(response_EM[index_test])
file["response_R1"]<- as.matrix(response_R1[index_test])
file["response_R3"]<- as.matrix(response_R3[index_test])
file["response_R5"]<- as.matrix(response_R5[index_test])
file["response_R7"]<- as.matrix(response_R7[index_test])
file["response_R8"]<- as.matrix(response_R8[index_test])

file["species"]<- species[index_test]
h5close(file)



dfile <- paste0('vigna-2021-v4-vqtl-utf-tr-val.h5')
file <- h5file(dfile)

file["year"] <- as.matrix(as.integer(year[index_tr_val]))
file["doy"]   <- as.matrix(as.integer(doy[index_tr_val]))
file["month"] <- as.matrix( as.integer(month[index_tr_val]))
file["geo_id"] <-as.matrix( as.integer(geo_id[index_tr_val]))

#gr_names <- c("snp1", "snp2","snp3","snp4","snp5","snp6","snp7","snp8","snp9","snp10")
file["gr_names"]<- gr_names
file["gr_covar"] <- as.matrix(gr_covar[index_tr_val,])
file["response_EM"]<- as.matrix(response_EM[index_tr_val])
file["response_R1"]<- as.matrix(response_R1[index_tr_val])
file["response_R3"]<- as.matrix(response_R3[index_tr_val])
file["response_R5"]<- as.matrix(response_R5[index_tr_val])
file["response_R7"]<- as.matrix(response_R7[index_tr_val])
file["response_R8"]<- as.matrix(response_R8[index_tr_val])

file["species"]<- species[index_tr_val]
h5close(file)


dfile <- paste0('vigna-2021-v3-vqtl-utf.h5')
file <- h5file(dfile)

year <- file["year"][]
doy = file["doy"][]
month = file["month"][]
geo_id <-file["geo_id"][]


gr_names <-  file["gr_names"][]
gr_covar <- file["gr_covar"][]
response_EM <- file["response_EM"][]
response_R1 <- file["response_R1"][]
response_R3 <- file["response_R3"][]
response_R5 <- file["response_R5"][]
response_R7 <- file["response_R7"][]
response_R8 <- file["response_R8"][]

species <-file["species"][]
h5close(file)

dfile <- paste0('vigna-2021-v3-vqtl-utf-test.h5')
file <- h5file(dfile)

file["year"] <- as.matrix(as.integer(year[index_test]))
file["doy"]   <- as.matrix(as.integer(doy[index_test]))
file["month"] <- as.matrix( as.integer(month[index_test]))
file["geo_id"] <-as.matrix( as.integer(geo_id[index_test]))

#gr_names <- c("snp1", "snp2","snp3","snp4","snp5","snp6","snp7","snp8","snp9","snp10")
file["gr_names"]<- gr_names
file["gr_covar"] <- as.matrix(gr_covar[index_test,])
file["response_EM"]<- as.matrix(response_EM[index_test])
file["response_R1"]<- as.matrix(response_R1[index_test])
file["response_R3"]<- as.matrix(response_R3[index_test])
file["response_R5"]<- as.matrix(response_R5[index_test])
file["response_R7"]<- as.matrix(response_R7[index_test])
file["response_R8"]<- as.matrix(response_R8[index_test])

file["species"]<- species[index_test]
h5close(file)



dfile <- paste0('vigna-2021-v3-vqtl-utf-tr-val.h5')
file <- h5file(dfile)

file["year"] <- as.matrix(as.integer(year[index_tr_val]))
file["doy"]   <- as.matrix(as.integer(doy[index_tr_val]))
file["month"] <- as.matrix( as.integer(month[index_tr_val]))
file["geo_id"] <-as.matrix( as.integer(geo_id[index_tr_val]))

#gr_names <- c("snp1", "snp2","snp3","snp4","snp5","snp6","snp7","snp8","snp9","snp10")
file["gr_names"]<- gr_names
file["gr_covar"] <- as.matrix(gr_covar[index_tr_val,])
file["response_EM"]<- as.matrix(response_EM[index_tr_val])
file["response_R1"]<- as.matrix(response_R1[index_tr_val])
file["response_R3"]<- as.matrix(response_R3[index_tr_val])
file["response_R5"]<- as.matrix(response_R5[index_tr_val])
file["response_R7"]<- as.matrix(response_R7[index_tr_val])
file["response_R8"]<- as.matrix(response_R8[index_tr_val])

file["species"]<- species[index_tr_val]
h5close(file)

