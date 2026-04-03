package com.jgezalian.ballistics_backend.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.HttpStatusEntryPoint;
import org.springframework.http.HttpStatus;
import org.springframework.web.cors.CorsConfiguration;
import org.springframework.web.cors.UrlBasedCorsConfigurationSource;
import org.springframework.security.config.Customizer;
import java.util.Arrays;
// import org.slf4j.Logger;
// import org.slf4j.LoggerFactory;

@Configuration
public class OAuth2Login {

        private final String frontEndOrigin;

        OAuth2Login(@Value("${app.frontend.origin}") String frontEndOrigin) {
                this.frontEndOrigin = frontEndOrigin;
        }

        @Bean
        UrlBasedCorsConfigurationSource corsConfigurationSource() {
                CorsConfiguration configuration = new CorsConfiguration();
                configuration.setAllowedOrigins(Arrays.asList(frontEndOrigin));
                configuration.setAllowedMethods(Arrays.asList("GET", "POST", "DELETE"));
                UrlBasedCorsConfigurationSource source = new UrlBasedCorsConfigurationSource();
                source.registerCorsConfiguration("/**", configuration);
                return source;
        }

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
                                .oauth2Login(oauth2 -> oauth2.defaultSuccessUrl(frontEndOrigin, true));

                http.csrf((csrf) -> csrf.spa());
                http.logout(l -> l
                                .logoutSuccessUrl(frontEndOrigin).permitAll());

                return http.build();
        }

}
