/*
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2014 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <librfn/console.h>

#include <string.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <librfn/time.h>
#include <librfn/util.h>

enum {
	on = 0,
	off,
	toggle,
	pulse,
	usage
};

static int parse_args(console_t *c, console_gpio_t *gpio)
{
	int action;

	/* argument parsing */
	if (0 == strcmp(c->argv[1], "on"))
		action = on;
	else if (0 == strcmp(c->argv[1], "off"))
		action = off;
	else if (0 == strcmp(c->argv[1], "toggle"))
		action = toggle;
	else if (0 == strcmp(c->argv[1], "pulse"))
		action = pulse;
	else
		action = usage;

	/* update any flags */
	if ((action <= off) && (gpio->flags & console_gpio_active_low))
		action = !action;

	return action;
}

/* This section is a rather nasty workaround until I figure out how to get
 * open drain pins to work correctly on STM32F4.
 */
#ifdef STM32F4
static void do_opendrain_toggle(uintptr_t port, uint32_t pin)
{
	for (int i=0; i<32; i++) {
		if (pin & (1 << i)) {
			if (GPIO_MODER(port) & GPIO_MODE_MASK(i))
				gpio_mode_setup(port, GPIO_MODE_INPUT,
						GPIO_PUPD_NONE, 1 << i);
			else
				gpio_mode_setup(port, GPIO_MODE_OUTPUT,
						GPIO_PUPD_NONE, 1 << i);
		}
	}
}

static pt_state_t do_opendrain_cmd(console_t *c)
{
	console_gpio_t *gpio = containerof(c->cmd, console_gpio_t, cmd);
	uint32_t *t = &c->scratch.u32[0];

	PT_BEGIN(&c->pt);

	int action = parse_args(c, gpio);
	if (action == on) {
		gpio_mode_setup(gpio->port, GPIO_MODE_INPUT, GPIO_PUPD_NONE,
				gpio->pin);
	} else if (action == off) {
		gpio_mode_setup(gpio->port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
				gpio->pin);
	} else if (action == toggle) {
		do_opendrain_toggle(gpio->port, gpio->pin);
	} else if (action == pulse) {
		do_opendrain_toggle(gpio->port, gpio->pin);
		*t = time_now() + 1000000;
		PT_WAIT_UNTIL(fibre_timeout(*t));
		do_opendrain_toggle(gpio->port, gpio->pin);
	} else {
		fprintf(c->out, "Usage: %s on|off|toggle|pulse\n",
			c->cmd->name);
	}

	PT_END();
}
#endif

pt_state_t console_gpio_do_cmd(console_t *c)
{
	console_gpio_t *gpio = containerof(c->cmd, console_gpio_t, cmd);
	uint32_t *t = &c->scratch.u32[0];

#ifdef STM32F4
	if (gpio->flags & console_gpio_open_drain)
		return do_opendrain_cmd(c);
#endif

	PT_BEGIN(&c->pt);

	int action = parse_args(c, gpio);
	if (action == on) {
		gpio_set(gpio->port, gpio->pin);
	} else if (action == off) {
		gpio_clear(gpio->port, gpio->pin);
	} else if (action == toggle) {
		gpio_toggle(gpio->port, gpio->pin);
	} else if (action == pulse) {
		gpio_toggle(gpio->port, gpio->pin);
		*t = time_now() + 1000000;
		PT_WAIT_UNTIL(fibre_timeout(*t));
		gpio_toggle(gpio->port, gpio->pin);
	} else {
		fprintf(c->out, "Usage: %s on|off|toggle|pulse\n",
			c->cmd->name);
	}

	PT_END();
}

int console_gpio_register(const console_gpio_t *gpio)
{
	switch (gpio->port) {
#ifdef GPIOA
	case GPIOA:
		rcc_periph_clock_enable(RCC_GPIOA);
		break;
#endif
#ifdef GPIOB
	case GPIOB:
		rcc_periph_clock_enable(RCC_GPIOB);
		break;
#endif
#ifdef GPIOC
	case GPIOC:
		rcc_periph_clock_enable(RCC_GPIOC);
		break;
#endif
#ifdef GPIOD
	case GPIOD:
		rcc_periph_clock_enable(RCC_GPIOD);
		break;
#endif
#ifdef GPIOE
	case GPIOE:
		rcc_periph_clock_enable(RCC_GPIOE);
		break;
#endif
#ifdef GPIOF
	case GPIOF:
		rcc_periph_clock_enable(RCC_GPIOF);
		break;
#endif
	}

	if (!!(gpio->flags & console_gpio_active_low) ^
	    !!(gpio->flags & console_gpio_default_on))
		gpio_set(gpio->port, gpio->pin);
	else
		gpio_clear(gpio->port, gpio->pin);

#if defined(STM32F1)
	int mode = GPIO_MODE_OUTPUT_2_MHZ;
	int cnf = GPIO_CNF_OUTPUT_PUSHPULL;
	if (gpio->flags & console_gpio_open_drain)
		cnf = GPIO_CNF_OUTPUT_OPENDRAIN;
	gpio_set_mode(gpio->port, mode, cnf, gpio->pin);
#elif defined(STM32F4)
	int mode = GPIO_MODE_OUTPUT;
	if (gpio->flags & console_gpio_open_drain)
		mode = GPIO_MODE_INPUT;
	gpio_mode_setup(gpio->port, mode, GPIO_PUPD_NONE, gpio->pin);
#else
#error Unsupported part
#endif

	return console_register(&gpio->cmd);
}

