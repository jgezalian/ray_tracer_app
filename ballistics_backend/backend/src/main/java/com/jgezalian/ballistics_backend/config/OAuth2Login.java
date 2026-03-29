package com.jgezalian.ballistics_backend.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.HttpStatusEntryPoint;
// import org.springframework.security.web.csrf.CookieCsrfTokenRepository;
import org.springframework.http.HttpStatus;

import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.security.config.Customizer;
// import org.slf4j.Logger;
// import org.slf4j.LoggerFactory;

@Configuration
@CrossOrigin(origins = "http://localhost:4200", allowCredentials = "true")
public class OAuth2Login {
        // private static final Logger log = LoggerFactory.getLogger(OAuth2Login.class);

        @Bean
        SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {
                http.cors(Customizer.withDefaults());
                http
                                .authorizeHttpRequests(auth -> auth
                                                .requestMatchers("/login", "/error", "/oauth2/**", "/login/oauth2/**")
                                                .permitAll()
                                                .anyRequest().authenticated())
                                .exceptionHandling(e -> e
                                                .authenticationEntryPoint(
                                                                new HttpStatusEntryPoint(HttpStatus.UNAUTHORIZED)))
                                .oauth2Login(oauth2 -> oauth2.defaultSuccessUrl("http://localhost:4200", true));

                http.csrf((csrf) -> csrf.spa());
                http.logout(l -> l
                                .logoutSuccessUrl("http://localhost:4200").permitAll());
                // http.exceptionHandling(e -> e
                // .accessDeniedHandler((req, res, ex) -> {
                // log.warn("403 {} {} ex={}", req.getMethod(), req.getRequestURI(),
                // ex.toString());
                // res.sendError(403);
                // })
                // .authenticationEntryPoint(new
                // HttpStatusEntryPoint(HttpStatus.UNAUTHORIZED)));
                // http.csrf((csrf) -> csrf
                // .csrfTokenRepository(CookieCsrfTokenRepository.withHttpOnlyFalse()));

                return http.build();
        }

}
