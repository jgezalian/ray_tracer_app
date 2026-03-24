package com.jgezalian.ballistics_backend;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;

@SpringBootApplication
@EnableAutoConfiguration
@ComponentScan(basePackages = {"com.jgezalian.ballistics_backend"})
public class BallisticsBackendApplication {


	
	public static void main(String[] args) {
		SpringApplication.run(BallisticsBackendApplication.class, args);
	}

}





// 1. set region
// 2. aws sso login --profile jg-dev
// 3. export AWS_PROFILE=jg-dev  