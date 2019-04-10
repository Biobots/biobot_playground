import configparser
import logging
import telegram
from flask import Flask, request
from telegram.ext import CommandHandler
from telegram.ext import Dispatcher, MessageHandler, Filters, Updater
from myparser import *

config = configparser.ConfigParser()
config.read('config.ini')


import logging
logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.INFO)

def geturl(update, context):
    res = getRes(context.args[0])
    if res.getcode() == 200:
        print(res.read().decode("utf-8"))
        update.message.reply_text(text="res.info()")
    else:
        update.message.reply_text(text="fail")
    

def alarm(context):
    job = context.job
    context.bot.send_message(job.context, text='Timer test!')

def start(update, context):
    update.message.reply_text(text = "I'm a bot, please talk to me!")
    module_test("CaoHigh")

def echo(update, context):
    context.bot.send_message(chat_id=update.message.chat_id, text=update.message.text)

def set_timer(update, context):
    chat_id = update.message.chat_id
    if len(context.args) == 0:
        due = 10
    else:
        due = int(context.args[0])
    if due < 0:
        update.message.reply_text('exm?')
        return
    job = context.job_queue.run_repeating(alarm, due, context=chat_id)
    context.chat_data['job'] = job
    update.message.reply_text('Timer successfully set!')

def unset_timer(update, context):
    if 'job' not in context.chat_data:
        update.message.reply_text('You have no active timer')
        return
    job = context.chat_data['job']
    job.schedule_removal()
    del context.chat_data['job']
    update.message.reply_text('Timer successfully unset!')


updater = Updater(token = config['Telegram']['Access_Token'], use_context = True)
dispatcher = updater.dispatcher

res_handler = CommandHandler('get', geturl)
dispatcher.add_handler(res_handler)
start_handler = CommandHandler('start', start)
dispatcher.add_handler(start_handler)
echo_handler = CommandHandler('echo', echo)
dispatcher.add_handler(echo_handler)
dispatcher.add_handler(CommandHandler("set", set_timer,
                                  pass_args=True,
                                  pass_job_queue=True,
                                  pass_chat_data=True))
dispatcher.add_handler(CommandHandler("unset", unset_timer, pass_chat_data=True))


updater.start_polling()
updater.idle()